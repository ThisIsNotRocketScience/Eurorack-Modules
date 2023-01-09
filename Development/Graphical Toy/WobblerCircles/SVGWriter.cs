//using GerberLibrary.Core.Primitives;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class PointD
{
    public double X;
    public double Y;

    public PointD(PointF pointF)
    {
        X = pointF.X;
        Y = pointF.Y;
    }
    public PointD(double _x = 0, double _y = 0)
    {
        X = _x;
        Y = _y;
    }
    public PointD(float _x, float _y)
    {
        X = _x;
        Y = _y;
    }

}


public class vec2
{
    public override string ToString()
    {
        return String.Format("({0},{1})", x, y);
    }
    public vec2(double _x, double _y)
    {
        x = (float)_x;
        y = (float)_y;
    }
    public vec2()
    {
        x = y = 0;
    }
    public float x;
    public float y;

    public static vec2 operator +(vec2 c1, vec2 c2)
    {
        vec2 temp = new vec2();
        temp.x = c1.x + c2.x;
        temp.y = c1.y + c2.y;
        return temp;
    }
    public static vec2 operator -(vec2 c1, vec2 c2)
    {
        vec2 temp = new vec2();
        temp.x = c1.x - c2.x;
        temp.y = c1.y - c2.y;
        return temp;

    }
    public static vec2 operator *(vec2 c1, float c2)
    {
        vec2 temp = new vec2();
        temp.x = c1.x * c2;
        temp.y = c1.y * c2;
        return temp;

    }
    public double length()
    {
        return Math.Sqrt(x * x + y * y);
    }
    public void Normalize()
    {
        double L = length();
        x /= (float)L;
        y /= (float)L;
    }
    public vec2 Normalized()
    {
        return this * (float)(1 / length());
    }

    public double Angle()
    {
        return Math.Atan2(y, x);
    }
}
public enum Ordering
{
    Base,
    SilkPrint,
    Carves,
    Holes,
    Standoffs
}

public class Polygon
{

    public byte r, g, b, a;
    public List<vec2> Vertices = new List<vec2>();
    public int Type = 0;
    public int depth = 0;
    internal bool divided = false;
    public bool Closed = true;
    public bool Filled = true;
    public string Source = "";
    Polygon()
    {
        Source = "Unknown";
    }


    public Polygon(string src)
    {
        Source = src;
    }
    public Ordering ordering = Ordering.Base;
    public bool Stroke = false;
    public double StrokeWidth;
    public byte strokeR;
    public byte strokeG;
    public byte strokeB;
    public byte strokeA;

    public void Draw(Graphics G, float linewidth = 1.0f)
    {
        List<PointF> Points = new List<PointF>();
        foreach (var a in Vertices)
        {
            Points.Add(new PointF(a.x, a.y));

        }
        if (Filled)
        {
            G.FillPolygon(new SolidBrush(Color.FromArgb(a, r, g, b)), Points.ToArray());
        }
        else
        {
            if (Closed)
            {
                G.DrawPolygon(new Pen(Color.FromArgb(a, r, g, b), linewidth), Points.ToArray());
            }
            else
            {
                G.DrawLines(new Pen(Color.FromArgb(a, r, g, b), linewidth), Points.ToArray());
            }
        }
    }
    public List<PointF> GetPointList()
    {
        List<PointF> R = new List<PointF>();

        foreach (var a in Vertices)
        {
            R.Add(new PointF((float)a.x, (float)a.y));
        }
        return R;
    }

    public static List<Polygon> FromListOfLists(List<List<PointF>> list, string SourceName)
    {
        List<Polygon> R = new List<Polygon>();
        foreach (var L in list)
        {
            R.Add(Polygon.FromList(L, SourceName));
        }

        return R;
    }

    public static Polygon FromList(List<PointF> l, string SourceName)
    {
        Polygon P = new Polygon(SourceName);
        foreach (var v in l)
        {
            P.Vertices.Add(new vec2() { x = v.X, y = v.Y });
        }
        return P;
    }

    internal void Translate(float dx, float dy)
    {
        foreach (var v in Vertices)
        {
            v.x += dx;
            v.y += dy;
        }

    }

    internal void SetColor(Color c)
    {
        r = c.R;
        g = c.G;
        b = c.B;
        a = c.A;
    }

    internal void MakeRoundedRect(float x1, float y1, float W, float H, float R, int blockRoundingSegments)
    {
        Vertices.Clear();

        Vertices.Add(new vec2(x1 + R, y1));
        Vertices.Add(new vec2(x1 + W - R, y1));

        AddArc(x1 + W - R, y1 + R, R, blockRoundingSegments, Math.PI, Math.PI / 2);

        Vertices.Add(new vec2(x1 + W, y1 + R));
        Vertices.Add(new vec2(x1 + W, y1));

        AddArc(x1 + W - R, y1 + H - R, R, blockRoundingSegments, Math.PI / 2, 0);

        Vertices.Add(new vec2(x1 + W, y1 + H - R));
        Vertices.Add(new vec2(x1 + W - R, y1 + H));

        AddArc(x1 + R, y1 + H - R, R, blockRoundingSegments, 0, -Math.PI / 2);

        Vertices.Add(new vec2(x1 + R, y1 + H));
        Vertices.Add(new vec2(x1, y1 + H - R));

        AddArc(x1 + R, y1 + R, R, blockRoundingSegments, -Math.PI / 2, -Math.PI);

        Vertices.Add(new vec2(x1, y1 + R));


        Closed = true;

    }

    private void AddArc(float cx, float cy, float r, int blockRoundingSegments, double A1, double A2)
    {
        for (int i = 0; i <= blockRoundingSegments; i++)
        {
            double P = A1 + (A2 - A1) * i / (double)blockRoundingSegments;
            Vertices.Add(new vec2(cx + Math.Sin(P) * r, cy + Math.Cos(P) * r));
        }
    }

    public void MakeRect(double x1, double y1, double x2, double y2)
    {
        Vertices.Clear();

        Vertices.Add(new vec2(x1, y1));
        Vertices.Add(new vec2(x2, y1));
        Vertices.Add(new vec2(x2, y2));
        Vertices.Add(new vec2(x1, y2));
        Closed = true;
    }

    public void MakeCircle(double x, double y, double r, int blockRoundingSegments)
    {
        Vertices.Clear();
        for (int i = 0; i < blockRoundingSegments; i++)
        {
            double P = (i * Math.PI * 2) / blockRoundingSegments;
            Vertices.Add(new vec2(Math.Sin(P) * r + x, Math.Cos(P) * r + y));
        }
        Closed = true;

    }

    internal vec2 GetCenter()
    {
        vec2 R = new vec2();
        if (Vertices.Count == 0) return R;
        R.x = Vertices[0].x;
        R.y = Vertices[0].y;
        for (int i = 1; i < Vertices.Count; i++)
        {
            R.x += Vertices[i].x;
            R.y += Vertices[i].y;
        }
        float F = 1.0f / (float)Vertices.Count;
        R.x *= F;
        R.y *= F;

        return R;
    }


    public vec2 GetSize()
    {
        vec2 R = new vec2();
        if (Vertices.Count < 2) return R;

        vec2 min = new vec2(Vertices[0].x, Vertices[0].y);
        vec2 max = new vec2(Vertices[0].x, Vertices[0].y);

        for (int i = 1; i < Vertices.Count; i++)
        {
            if (Vertices[i].x > max.x) max.x = Vertices[i].x; else if (Vertices[i].x < min.x) min.x = Vertices[i].x;
            if (Vertices[i].y > max.y) max.y = Vertices[i].y; else if (Vertices[i].y < min.y) min.y = Vertices[i].y;
        }
        R.x = max.x - min.x;
        R.y = max.y - min.y;
        return R;
    }

    public bool containsPoint(PointD testPoint)
    {


        bool result = false;
        int j = Vertices.Count() - 1;
        for (int i = 0; i < Vertices.Count(); i++)
        {
            if (Vertices[i].y < testPoint.Y && Vertices[j].y >= testPoint.Y || Vertices[j].y < testPoint.Y && Vertices[i].y >= testPoint.Y)
            {
                if (Vertices[i].x + (testPoint.Y - Vertices[i].y) / (Vertices[j].y - Vertices[i].y) * (Vertices[j].x - Vertices[i].x) < testPoint.X)
                {
                    result = !result;
                }
            }
            j = i;
        }
        return result;


        //            var v = (from i in Vertices select new PointD(i.x, i.y)).ToList();
        //          return GerberLibrary.Core.Helpers.IsInPolygon(v, p0);            
    }

    public PointD getCentroid()
    {
        var g = GetCenter();
        return new PointD(g.x, g.y);

    }
}

public class TextInstance
{
    public TextInstance()
    {

    }
    public TextInstance(string t, vec2 p, Color c, StringAlignment sa = StringAlignment.Center, float fs = 4, int l = 0)
    {
        layer = l;
        text = t;
        pos = p;
        color = c;
        Align = sa;
        fontsize = fs;
    }

    public Ordering ordering = Ordering.SilkPrint;
    public StringAlignment Align = StringAlignment.Near;
    public string text;
    public double fontsize = 12;
    public vec2 pos;
    public Color color;
    public int layer;
    internal string orig;
    internal string owner;

    public StringAlignment VerticalAlign = StringAlignment.Near;

    public override string ToString()
    {
        return text + String.Format(" {0},{1}", pos.x, pos.y);
    }
}

public class SVGWriter
{

    public static string N(double val, string unit = "mm")
    {
        return val.ToString().Replace(',', '.') + unit;
    }

    public static string N2(double val, string unit = "mm")
    {
        return val.ToString().Replace(',', '.');
    }

    public static string XmlEscape(string unescaped)
    {
        XmlDocument doc = new XmlDocument();
        XmlNode node = doc.CreateElement("root");
        node.InnerText = unescaped;
        return node.InnerXml;
    }


    public static void Write(string filename, float w, float h, List<Polygon> Polygons, double strokewidth, Color BG, List<TextInstance> texts, double fontsize = 4, string unit = "mm", float xoff = 0, float yoff = 0, int Layer = -1)
    {
        string bgcolorstring = String.Format("#{0:X2}{1:X2}{2:X2}", BG.R, BG.G, BG.B);

        List<string> OutLines = new List<string>();

        OutLines.Add("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\" >");
        OutLines.Add(String.Format("<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:space=\"preserve\" width=\"{0}\" height=\"{1}\">", N(w, unit), N(h, unit)));
        string fontstring = MakeFontstring("Panton", fontsize, StringAlignment.Center); ;
        OutLines.Add(String.Format(" <style> .small {{ {0} }}</style> ", fontstring));
        //OutLines.Add(String.Format("<rect width=\"100%\" height=\"100%\" fill=\"{0}\" />", bgcolorstring));
        OutLines.Add("<g transform=\"scale(2.8346456)\">");
        Dictionary<int, List<string>> groups = new Dictionary<int, List<string>>();
        Dictionary<int, List<string>> textgroups = new Dictionary<int, List<string>>();
        for (int i = 0; i < 256; i++)
        {
            groups[i] = new List<string>();
            textgroups[i] = new List<string>();
        }
        List<string> colors = new List<string>();// { "#606060", "#505050", "#404040", "#303030", "#202020", "#101010", "#080808", "#040404", "#020202", "#010101", "#000000", "#000000", "#000000", "#000000", "#000000", "#000000", "#000000", "#000000", "#000000" };
        
        if (Polygons != null)
        foreach (var a in Polygons)
        {
            if (Layer < 0 || (int)a.ordering == Layer || a.ordering == Ordering.Base)
                if (a.Vertices.Count > 0)
                {
                    string colorstring = String.Format("#{0:X2}{1:X2}{2:X2}", a.r, a.g, a.b);
                    string commands = "";
                    commands += "M" + N2(a.Vertices[0].x + xoff, unit) + "," + N2(a.Vertices[0].y + yoff, unit);
                    for (int i = 1; i < a.Vertices.Count; i++)
                    {
                        commands += "L" + N2(a.Vertices[i].x + xoff, unit) + "," + N2(a.Vertices[i].y + yoff, unit);
                    }
                    if (a.Closed)
                    {
                        commands += "L" + N2(a.Vertices[0].x + xoff, unit) + "," + N2(a.Vertices[0].y + yoff, unit);
                        commands += "Z";
                    }
                    if (a.Filled)
                    {
                        string setup = String.Format("<path stroke=\"none\" fill=\"{2}\" stroke-width=\"{0}\" stroke-linecap=\"round\" stroke-linejoin=\"round\" d=\"{1}\"/>", N(strokewidth, unit), commands, colorstring);
                        groups[(int)a.ordering].Add(setup);
                    }
                    else
                    {
                        string setup = String.Format("<path fill=\"none\" stroke=\"{2}\" stroke-width=\"{0}\" stroke-linecap=\"round\" stroke-linejoin=\"round\" d=\"{1}\"/>", N(a.StrokeWidth>0?a.StrokeWidth:strokewidth, unit), commands, colorstring);
                        groups[(int)a.ordering].Add(setup);

                    }
                }
        }
        if (texts != null)
        foreach (var t in texts)
        {
            if (Layer < 0 || (int)t.ordering == Layer)
            {
                var a = t.color;
                string colorstring = String.Format("#{0:X2}{1:X2}{2:X2}", a.R, a.G, a.B);
                string fontstring2 = MakeFontstring("Panton", t.fontsize, t.Align); ;

                textgroups[t.layer].Add(string.Format("<text x=\"{0}\" y=\"{1}\" class=\"small\" style=\"{4}\" fill=\"{3}\">{2}</text>", N2(t.pos.x + xoff, unit), N2(t.pos.y + yoff, unit), XmlEscape(t.text), colorstring, fontstring2));
            }
        }

        foreach (var a in groups)
        {
            var L = a.Value;
            var T = textgroups[a.Key];
            if (L.Count > 0 || T.Count > 0)
            {
                OutLines.Add("<g>");
                foreach (var p in L) OutLines.Add(p);
                OutLines.Add("<g>");
                foreach (var p in T) OutLines.Add(p);
                OutLines.Add("</g>");
                OutLines.Add("</g>");
            }
        }
        OutLines.Add("<g>");

        OutLines.Add("</g>");

        OutLines.Add("</g>");
        OutLines.Add("</svg>");
        System.IO.File.WriteAllLines(filename, OutLines);
    }

    private static string MakeFontstring(string v, double fontsize, StringAlignment center, string unit = "mm")
    {
        return String.Format("font: {0} {2}; text-anchor:{1} ;font-weight:bold;", N(fontsize / 2.8346456f, unit), Alignment(center), v);
    }

    private static string Alignment(StringAlignment Align)
    {
        switch (Align)
        {
            case StringAlignment.Center: return "middle";
            case StringAlignment.Near: return "start";
            case StringAlignment.Far: return "end";
        }
        return "middle";
    }
}

