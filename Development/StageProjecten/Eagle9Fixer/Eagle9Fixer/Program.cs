using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;


namespace Eagle9Fixer
{
    class Program
    {
        static void Main(string[] args)
        {

            if (args.Count() < 1)
            {
                Console.WriteLine("needs filename arg!");
                return;
            }

            var Folder = Path.GetDirectoryName(args[0]);
            var Name = Path.GetFileNameWithoutExtension(args[0]);
            var lines = File.ReadAllLines(args[0]);
            List<string> Outlines = new List<string>();
            Console.WriteLine("Folder: {0} Filename: {1}", Folder, Name);

            File.Copy(Path.Combine(Folder, Name + ".sch"), Path.Combine(Folder, Name + "_v9fixed.sch"), true);

            int InElement = 0;
            Dictionary<string, bool> CurrentNames = new Dictionary<string, bool>();

            foreach (var l in lines)
            {

                bool Keep = true;

                if (InElement == 2)
                {
                    if (l.StartsWith("<attribute"))
                    {
                        var S = l.Split(' ');
                        int i = 0;
                        Dictionary<string, bool> D = new Dictionary<string, bool>();
                        foreach(var s in S)
                        {
                            if (s.StartsWith("name="))
                            {
                                if (CurrentNames.ContainsKey(s)) Keep = false;
                                CurrentNames[s] = true;
                                    
                            }
                            D[s] = true;
                            i++;
                        }
                        if (D.ContainsKey("name=\"DESC\"") && D.ContainsKey("layer=\"254\"")) Keep = false;
                        if (D.ContainsKey("name=\"DESC\"") && D.ContainsKey("layer=\"27\"")) Keep = false;
                    }
                    
                }
                if (l.StartsWith("<element") && l.EndsWith("/>") == false ) InElement++;


                if (l.StartsWith("</element")) { InElement--; CurrentNames.Clear(); }

                if (Keep) Outlines.Add(l);

            }


            File.WriteAllLines(Path.Combine(Folder, Name + "_v9fixed.brd"), Outlines);

            Console.ReadKey();
        }
    }
}
