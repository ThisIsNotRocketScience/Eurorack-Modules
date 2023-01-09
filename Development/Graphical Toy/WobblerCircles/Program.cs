using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.InteropServices;

using System.Drawing;

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
    
    class WaveCollapser
    {
        List<List<int>> Tiles = new List<List<int>>();
        public void Doit(string filename, int w, int h)
        {
            Tiles.Clear();
            for(int y = 0;y<h;y++)
            {
                var XL = new List<int>();
                for (int x =0;x<w;x++)
                {
                    XL.Add(-1);
                }
                Tiles.Add(XL);
            }

            List<Polygon> Things = new List<Polygon>();

            Things.AddRange(DrawTile(1, 10, 0));
            Things.AddRange(DrawTile(2, 10, 1));
            Things.AddRange(DrawTile(3, 10, 2));

            SVGWriter.Write(filename, w*10, h*10, Things, 0.1, Color.DarkBlue, null);

        }

        private List<Polygon> DrawTile(int x, int y, int tile)
        {
            List<Polygon> r = new List<Polygon>();

            switch(tile)
            {
                case 0:
                    Polygon P = new Polygon("tile0");
                    P.MakeRoundedRect(x * 10 + 3, y * 10 + 5, 6, 2, 0.6f, 3);
                    r.Add(P);
                    break;
                case 1:
                    Polygon P2 = new Polygon("tile1");
                    P2.MakeRoundedRect(x * 10 + 5, y * 10 + 3, 2, 6, 0.6f, 3);
                    r.Add(P2);
                    break;
                case 2:
                    Polygon P3 = new Polygon("tile2");
                    P3.MakeRoundedRect(x * 10 + 3, y * 10 + 5, 6, 2, 0.6f, 3);
                    r.Add(P3);
                    Polygon P4 = new Polygon("tile2");
                    P4.MakeRoundedRect(x * 10 + 5, y * 10 + 3, 2, 6, 0.6f, 3);
                    r.Add(P4);
                    break;
            }

            return r;
            
        }
    }
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
            //CircleGen();
            //HeaderGen();

            for (int i = 0; i < 10; i++)
            {
                //    MakeTree(String.Format("Classic{0}.svg", i), 100, 100, 2, -45, 90, 0.65f, 4+i);
            //    WaveCollapser WC = new WaveCollapser();
              //  WC.Doit(String.Format("WC{0}.svg", i),100,40);


            }

            CircleGen g = new CircleGen();
            g.Doit("circles.svg");


        }


        public class Branch
        {
            public vec2 Start = new vec2();
            public vec2 End = new vec2();
            
            public double Len ()
            {
                return (End - Start).length();
            }

            public double Angle()
            {
                var A = (End - Start).Angle();

                return (A/Math.PI)*180 - 0;
            }
            public static Random R = new Random();
            public List<Branch> Split(int count, double randomcenter,double randomscaler, double angleoffset, double spreadangle, double lengthmult)
            {
                List<Branch> res = new List<Branch>();
                double myangle = Angle();
                double newlen = lengthmult * Len();
                double randomoffset = 0;
                double randomness = Math.Min(Math.Max((Start.x - randomcenter) / randomscaler, 0.0f), 1.0f);
                randomness *= R.NextDouble() - 0.5f;
                for(int i = 0;i<count;i++)
                {
                    double newangle = angleoffset + i * (spreadangle / (double)(count - 1)) + randomoffset ;
                    randomoffset += randomness * spreadangle;
                    newangle += myangle;
                    double nA = (newangle * Math.PI * 2.0f) / 360.0f;
                    double newlenB = newlen + randomness * Len() / 10.0f;
                    Branch B = new Branch();
                    B.Start = End;
                    B.End = B.Start + new vec2(Math.Cos(nA) * newlenB, Math.Sin(nA) * newlenB);
                    res.Add(B);
                }
                
                return res;
            }
        }
            
        static void MakeTree(string outputname, float w, float h, int splits, double offsetangle, double spreadangle, double lengthmult, int maxdepth)
        {
            List<Polygon> Things = new List<Polygon>();

            Branch Start = new Branch();
            Start.Start.x = w / 2;
            Start.Start.y = h - h / 10;
            Start.End.x = w / 2 ;
            Start.End.y = h - h / 2;
            
            List<Branch> SubDivStack = new List<Branch>();
            SubDivStack.Add(Start);
            
            List<Branch> TotalBranch = new List<Branch>();
            TotalBranch.Add(Start);

            for (int i = 0; i < maxdepth; i++)
            {
                List<Branch> NewSubdivStack = new List<Branch>();
                foreach(var a in SubDivStack)
                {
                    var R = a.Split(splits, w/2, w/2, offsetangle + Branch.R.NextDouble()*20, spreadangle + Branch.R.NextDouble() * 20, lengthmult);
                    TotalBranch.AddRange(R);
                    NewSubdivStack.AddRange(R);
                }
                SubDivStack = NewSubdivStack;
            };

            foreach(var a in TotalBranch)
            {
                Polygon P = new Polygon("b");

                var B = a.End - a.Start;
                vec2 rot = new vec2(B.y, -B.x).Normalized(); ;

                rot *= (float)(B.length() / 10.0f );
                P.Vertices.Add(a.Start - rot);
                P.Vertices.Add(a.Start+rot);
                P.Vertices.Add(a.End+rot);
                P.Vertices.Add(a.End-rot);

                P.strokeA = 255;
                P.Stroke = true;
                P.StrokeWidth = 1;
                P.SetColor(Color.Black);
                Things.Add(P);
            }
            SVGWriter.Write(outputname, w, h, Things ,  0.1, Color.DarkBlue, null);


        }

        private static void HeaderGen()
        {
            List<string> Lines = new List<string>();

            Lines.AddRange(CreateLines("tick.raw", "tick"));
            List<int> Speeds = new List<int>();

            for (int i = 0; i < 128; i++)
            {

                double R = Math.Pow(2.0, (i - 69) / 12.0) * (double)(1 << 12);

                Speeds.Add((int)R);
            }
            Lines.AddRange(CreateIntTable(Speeds, "speedvals"));
            var CubicTable = interp();
            Lines.AddRange(CreateIntTable(CubicTable, "CubicTable"));

            File.WriteAllLines("tick.h", Lines.ToArray());
        }

        private static void CircleGen()
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

                double Radius = ((18.4 - 4.2) * (i / 119.0f) + 4.2 + Math.Sin(i * 0.2) * 0.3) * 4.0;


                P2.MakeCircle(CX, CY, Radius, 400);
                P2.Filled = false;

                P2.r = (byte)(255 * (i / 119.0f));
                P2.g = (byte)(255 * (i / 119.0f));
                P2.b = (byte)(255 - (byte)(255 * (i / 119.0f)));
                Circles.Add(P2);


            }

            SVGWriter.Write("Wobbler2Circles.svg", 50, 150, Circles, 0.1, System.Drawing.Color.DarkBlue, null);
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

    internal class CircleGen
    {
        public CircleGen()
        {
        }

        internal void Doit(string outputname, int w = 1000, int h = 1000)
        {

            List<Polygon> Things = new List<Polygon>();
            float X1 = -1.5f, Y1 = 113.5f, D1 = 58.5f;
            float X2 = 7.6f, Y2 = 112.4f, D2 = 12.0f;
            for (int i =0;i<20;i++)
            {
                float T = i / 19.0f;
                float nT = 1-(float)Math.Pow(1-T, 2);
                float X = (X2 - X1) * nT + X1;
                float Y = (Y2 - Y1) * nT + Y1;
                float R = (D2 - D1) * T + D1;
                Polygon P = new Polygon("circlegen");
                P.MakeCircle(X, Y, R / 2.0f, 150);
                P.Filled = false;
                P.Stroke = true;
                P.StrokeWidth = 0.01 + 0.14 * (nT);
                Things.Add(P);
                Polygon P2 = new Polygon("circlegen");
                P2.MakeCircle(60.96f-X, Y, R / 2.0f, 150);
                P2.Filled = false;
                P2.Stroke = true;
                P2.StrokeWidth =0.01+ 0.14*(nT);
                Things.Add(P2);
            }
            SVGWriter.Write(outputname, w, h, Things, 0.1, Color.DarkBlue, null);
            

        }
    }
}
