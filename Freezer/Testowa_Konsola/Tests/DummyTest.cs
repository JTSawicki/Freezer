using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Testowa_Konsola.Tests
{
    public class DummyTest : ConsoleTest
    {
        public DummyTest() : base("DummyTest", "Niespecjalnie robi coś użytecznego")
        {
            ;
        }
        public override void ExecuteTest()
        {
            WriteLine("DummyTest wita :)");
            WriteLine("koniec testu");

            int delay = 50;
            WriteLine($"Delay: {(((double)delay) / 1000).ToString(System.Globalization.CultureInfo.InvariantCulture)}");
        }
    }
}
