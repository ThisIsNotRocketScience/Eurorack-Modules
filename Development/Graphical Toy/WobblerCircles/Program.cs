using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Explicit)]
struct Converter
{
    [FieldOffset(0)]
    public ushort UshortValue;
    [FieldOffset(0)]
    public short ShortValue;
    [FieldOffset(0)]
    public byte LoByte;
    [FieldOffset(1)]
    public byte HiByte;
}

namespace WobblerCircles
{
    class Program
    {

        static List<int> interp()
        {
            List<int> Res = new List<int>();

            int totalcount = 64;
            for (int i = 0; i < totalcount; i++)
            {
                double t =(((double)(i+1)) / (float)totalcount);
                double t2 = t * t;
                double t3 = t * t * t;

                double A = (3 * t2 - t3 - 3 * t + 1);
                double B = (3 * t3 - 6 * t2 + 3 * t);
                double C = (3 * t2 - 3 * t3);
                double D = t3;

                Res.Add((int)Math.Round(A * 65536));
                Res.Add((int)Math.Round(B * 65536));
                Res.Add((int)Math.Round(C * 65536));
                Res.Add((int)Math.Round(D * 65536));
            }

            return Res;
        }

        static void Main(string[] args)
        {

            List<Polygon> Circles = new List<Polygon>();
            List<Polygon> Circles2 = new List<Polygon>();
                double CX = 25;
                double CY = 25;
            int pts = 1000;

            for (int i = 0; i < 6; i++)
            {
                Polygon P = new Polygon("circlegen");


                double Radius = (18.4 - 4.2) * (i / 5.0f) + 4.2;

                for (int j = 0; j < pts; j++)
                {
                    double phase = (j * Math.PI * 2) / (double)pts;
                    vec2 pt = new vec2(CX, CY);
                    float wob = (float)(Math.Sin(phase * 60) * .9);
                    pt.x += (float)(Math.Sin(phase) * (Radius + wob));
                    pt.y += (float)(Math.Cos(phase) * (Radius + wob));
                    P.Vertices.Add(pt);
                }
                //P.MakeCircle(CX, CY, Radius, 300);
                P.Filled = false;
                P.Closed = true;
                P.r = (byte)(255 * (i / 5.0f));
                P.g = (byte)(255 * (i / 5.0f));
                P.b = (byte)(255 - (byte)(255 * (i / 5.0f)));
                Circles.Add(P);

            }

            CY += 50;

            for (int i = 0; i < 12; i++)
            {
                Polygon P2 = new Polygon("circlegen");

                double Radius = (18.4 - 4.2) * (i / 11.0f) + 4.2;

                for (int j = 0; j < pts; j++)
                {
                    double phase = (j * Math.PI * 2) / (double)pts;
                    vec2 pt = new vec2(CX, CY);
                    float wob = 1.0f + (float)(Math.Abs(Math.Sin(phase * 8)) * 0.10);
                    pt.x += (float)(Math.Sin(phase) * (Radius * wob));
                    pt.y += (float)(Math.Cos(phase) * (Radius * wob));
                    P2.Vertices.Add(pt);
                }
                //P.MakeCircle(CX, CY, Radius, 300);
                P2.Filled = false;
                P2.Closed = true;
                P2.r = (byte)(255 * (i / 11.0f));
                P2.g = (byte)(255 * (i / 11.0f));
                P2.b = (byte)(255 - (byte)(255 * (i / 11.0f)));
                Circles.Add(P2);


            }
            CY += 50;

            for (int i = 0; i < 120; i++)
            {
                Polygon P2 = new Polygon("circlegen");

                double Radius = ((18.4 - 4.2) * (i / 119.0f) + 4.2 + Math.Sin(i*0.2)*0.3) * 4.0 ;


                P2.MakeCircle( CX, CY, Radius,  400);
                P2.Filled = false;

                P2.r = (byte)(255 * (i / 119.0f));
                P2.g = (byte)(255 * (i / 119.0f));
                P2.b = (byte)(255 - (byte)(255 * (i / 119.0f)));
                Circles.Add(P2);


            }

            SVGWriter.Write("Wobbler2Circles.svg", 50, 150, Circles, 0.1, System.Drawing.Color.DarkBlue, null);


        

            List<string> Lines = new List<string>();

            Lines.AddRange(CreateLines("tick.raw", "tick"));
            List<int> Speeds = new List<int>();

            for (int i =0;i<128;i++)
            {

                double R = Math.Pow(2.0, (i - 69) / 12.0)*(double)(1<<12);

                Speeds.Add((int)R);
            }
            Lines.AddRange(CreateIntTable(Speeds, "speedvals"));
            var CubicTable = interp();
            Lines.AddRange(CreateIntTable(CubicTable, "CubicTable"));

            File.WriteAllLines("tick.h", Lines.ToArray());

        }

        private static List<string> CreateShortTable(List<short> shorts, string name)
        {
            List<string> Lines = new List<string>();
            Lines.Add("#include <stdint.h>");
            Lines.Add("int " + name + "_count = " + (shorts.Count ).ToString() + ";");
            Lines.Add("int16_t " + name + "[" + (shorts.Count).ToString() + "] = {");
            string cur = "";
            for (int i = 0; i < shorts.Count; i ++)
            {
                var S= shorts[i];

                if (i < shorts.Count- 1)
                {
                    cur += String.Format("{0,6},", S);
                }
                else
                {
                    cur += String.Format("{0,6}", S);

                }
                if (i > 0 && i % 64 == 0)
                {
                    Lines.Add(cur);
                    cur = "";
                }

            }

            if (cur.Length > 0) Lines.Add(cur);
            Lines.Add("};");
            Lines.Add("");
            return Lines;
        }

        private static List<string> CreateIntTable(List<int> shorts, string name)
        {
            List<string> Lines = new List<string>();
            Lines.Add("#include <stdint.h>");
            Lines.Add("int " + name + "_count = " + (shorts.Count).ToString() + ";");
            Lines.Add("int32_t " + name + "[" + (shorts.Count).ToString() + "] = {");
            string cur = "";
            for (int i = 0; i < shorts.Count; i++)
            {
                var S = shorts[i];

                if (i < shorts.Count - 1)
                {
                    cur += String.Format("{0,6},", S);
                }
                else
                {
                    cur += String.Format("{0,6}", S);

                }
                if (i > 0 && i % 4 == 3)
                {
                    Lines.Add(cur);
                    cur = "";
                }

            }

            if (cur.Length > 0) Lines.Add(cur);
            Lines.Add("};");
            Lines.Add("");
            return Lines;
        }

        private static List<string> CreateLines(string v, string name)
        {
            List<string> Lines = new List<string>();

            var TickBytes = File.ReadAllBytes("tick.raw");
            Lines.Add("#include <stdint.h>");
            Lines.Add("int " + name + "_count = " + (TickBytes.Length / 2).ToString() + ";");
            Lines.Add("int16_t "+name+"[" + (TickBytes.Length / 2).ToString() + "] = {");
            string cur = "";
            for (int i = 0; i < TickBytes.Length; i += 2)
            {
                var B1 = TickBytes[i];
                var B2 = TickBytes[i + 1];

                short S = new Converter { HiByte = B2, LoByte = B1 }.ShortValue;

                if (i < TickBytes.Length - 2)
                {
                    cur += String.Format("{0,6},", S);
                }
                else
                {
                    cur += String.Format("{0,6}", S);

                }
                if (i>0 && i % 64 == 0)
                {
                    Lines.Add(cur);
                    cur = "";
                }

            }
            if (cur.Length > 0) Lines.Add(cur);
            Lines.Add("};");
            Lines.Add("");

            return Lines;
        }
    }
}
