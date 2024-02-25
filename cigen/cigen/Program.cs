using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Text;

internal partial class Program
{
	[CmdArg(Ordinal =0,Required = true,Description ="The input files",ItemName ="infile")]
	static IList<FileSystemInfo> Inputs { get; set; } = new List<FileSystemInfo>();
	[CmdArg(Name = "output")]
	static TextWriter Output { get; set; } = Console.Out;
	[CmdArg(Name = "threshold", Description = "The luminosity threshold (0-255, defaults to 127)")]
	static int Threshold = 127;
	static void Run()
	{
		var bitmaps = new List<Bitmap>();
		// the byte arrays containing the monoized pixel data
		var monos = new List<byte[]>();
		try
		{
			foreach (var fi in Inputs)
			{
				using (var stm = File.OpenRead(fi.FullName))
				{
					using (var img = Image.FromStream(stm))
					{
						bitmaps.Add(new Bitmap(img));
					}
				}
			}
			foreach (var bmp in bitmaps)
			{
				// pack the bitmap into the SSD1306 native framebuffer
				// format
				var bytes = new byte[bmp.Width * bmp.Height / 8];
				var i = 0;
				for (var yy = 0; yy < bmp.Height; yy += 8)
				{
					for (var x = 0; x < bmp.Width; ++x)
					{
						byte b = 0;
						for (var y = yy; y < yy + 8; ++y)
						{
							var oc = bmp.GetPixel(x, y);
							var white = ((int)((oc.R * 0.3) + (oc.G * 0.59) + (oc.B * 0.11))) > Threshold;
							if (white)
							{
								b |= (byte)(1 << ((y - yy)));
							}
						}
						bytes[i++] = b;
					}
				}
				monos.Add(bytes);
			}
		}
		finally
		{
			foreach (var bmp in bitmaps)
			{
				bmp.Dispose();
			}
		}
		var nname = GetFilename(Output);
		string cname;
		if (string.IsNullOrWhiteSpace(nname))
		{
			nname = "output.h";
			cname = "output";
		}
		else
		{
			cname = _MakeSafeName(Path.GetFileNameWithoutExtension(nname));
		}
		nname = _MakeSafeName(nname);
		Output.WriteLine("#ifndef {0}", nname.ToUpperInvariant());
		Output.WriteLine("#define {0}", nname.ToUpperInvariant());
		Output.WriteLine("#include <stdint.h>");
		Output.WriteLine("#include \"progmem.h\"");
		var names = new string[monos.Count];
		for (int i = 0; i < monos.Count; i++)
		{
			var mono = monos[i];
			Output.Write("const uint8_t ");
			Output.Write(cname + "_");
			names[i] = _MakeSafeName(Path.GetFileNameWithoutExtension(Inputs[i].Name));
			Output.Write(names[i]);
			Output.WriteLine("[] PROGMEM = {");
			// try each of the RLE mechanisms in turn:
			// this one RLE's both white and black runs (0x00 and 0xFF)
			var sb1 = new StringBuilder();
			var len1 = 0;
			var j = 0;
			while (j < mono.Length)
			{
				var b = mono[j];
				if (b == 0 || b == 255)
				{
					var ob = b;
					var count = 1;
					++j;
					while (count < 255 && j < mono.Length && b == ob)
					{
						b = mono[j];
						++count;
						++j;
					}
					sb1.Append("0x" + ob.ToString("x2"));
					sb1.Append(", ");
					sb1.Append(count.ToString());
					if (j < mono.Length - 1)
					{
						sb1.Append(", ");
					}
					len1 += 2;
					continue;
				}
				sb1.Append("0x" + b.ToString("x2"));
				if (j < mono.Length - 1)
				{
					sb1.Append(", ");
				}
				++j;
				++len1;
			}
			// next compression method
			// this one RLE's black runs only (0x00)
			var sb2 = new StringBuilder();
			var len2 = 0;
			j = 0;
			while (j < mono.Length)
			{
				var b = mono[j];
				if (b == 0)
				{
					var ob = b;
					var count = 1;
					++j;
					while (count < 255 && j < mono.Length && b == ob)
					{
						b = mono[j];
						++count;
						++j;
					}
					sb2.Append("0x" + ob.ToString("x2"));
					sb2.Append(", ");
					sb2.Append(count.ToString());
					if (j < mono.Length - 1)
					{
						sb2.Append(", ");
					}
					len2 += 2;
					continue;
				}
				sb2.Append("0x" + b.ToString("x2"));
				if (j < mono.Length - 1)
				{
					sb2.Append(", ");
				}
				++j;
				++len2;
			}
			// next compression method
			// this one RLE's white runs only (0x00)
			var sb3 = new StringBuilder();
			var len3 = 0;
			j = 0;
			while (j < mono.Length)
			{
				var b = mono[j];
				if (b == 0)
				{
					var ob = b;
					var count = 1;
					++j;
					while (count < 255 && j < mono.Length && b == ob)
					{
						b = mono[j];
						++count;
						++j;
					}
					sb3.Append("0x" + ob.ToString("x2"));
					sb3.Append(", ");
					sb3.Append(count.ToString());
					if (j < mono.Length - 1)
					{
						sb3.Append(", ");
					}
					len3 += 2;
					continue;
				}
				sb3.Append("0x" + b.ToString("x2"));
				if (j < mono.Length - 1)
				{
					sb3.Append(", ");
				}
				++j;
				++len3;
			}
			// next method
			// this one does no compression
			var sb4 = new StringBuilder();
			var len4 = 0;
			j = 0;
			while (j < mono.Length)
			{
				var b = mono[j];
				sb4.Append("0x" + b.ToString("x2"));
				if (j < mono.Length - 1)
				{
					sb4.Append(", ");
				}
				++j;
				++len4;
			}
			var compmethod = 3;
			var len = len1;
			var sb = sb1;
			if (len2 < len)
			{
				compmethod = 1;
				len = len2;
				sb = sb2;
			}
			if (len3 < len)
			{
				compmethod = 2;
				len = len3;
				sb = sb3;
			}
			if (len4 < len)
			{
				compmethod = 0;
				len = len4;
				sb = sb4;
			}

			Output.Write("    ");
			Output.WriteLine(WordWrap(sb.ToString(), 40, 4, 4));
			Output.WriteLine("};");
			Output.Write("#define ");
			Output.Write(cname.ToUpperInvariant() + "_");
			Output.Write(names[i].ToUpperInvariant() + "_COMPRESSION");
			Output.Write(' ');
			Output.WriteLine(compmethod.ToString());
			Console.Error.WriteLine();
			Console.Error.WriteLine("// [Compressed to {0}% of original. Len = {1} vs {2}]", ((double)len / (double)(mono.Length)) * 100, len, mono.Length);
			Console.Error.WriteLine();
		}
		Output.Write("const uint8_t* ");
		Output.Write(cname);
		Output.WriteLine("_images[] = {");

		for (int i = 0; i < monos.Count; i++)
		{
			var n = names[i];
			Output.Write("    ");
			Output.Write(cname + "_");
			Output.Write(n);
			if (i < monos.Count - 1)
			{
				Output.Write(',');
			}
			Output.WriteLine();
		}
		Output.WriteLine("};");
		Output.Write("const int ");
		Output.Write(cname);
		Output.WriteLine("_images_compression[] = {");

		for (int i = 0; i < monos.Count; i++)
		{
			var n = names[i];
			Output.Write("    ");
			Output.Write(cname.ToUpperInvariant() + "_");
			Output.Write(n.ToUpperInvariant() + "_COMPRESSION");
			if (i < monos.Count - 1)
			{
				Output.Write(',');
			}
			Output.WriteLine();
		}
		Output.WriteLine("};");

		Output.WriteLine("#endif // {0}", nname.ToUpperInvariant());
	}

	private static string _MakeSafeName(string nname)
	{
		if(string.IsNullOrEmpty(nname))
		{
			return nname;
		}
		var result = new StringBuilder(nname.Length);
		if(char.IsDigit(nname,0))
		{
			result.Append('_');
		}
		for (int i = 0; i < nname.Length; ++i)
		{
			if (!char.IsLetterOrDigit(nname,i))
			{
				result.Append('_');
			}
			else
			{
				result.Append(nname[i]);
			}
		}

		return result.ToString();
	}
}

