using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Testowa_Konsola
{
    public abstract class ConsoleTest
    {
        public string TestName { get; private set; }
        public string TestDescription { get; private set; }

        public ConsoleTest(string testName, string testDescription)
        {
            TestName = testName;
            TestDescription = testDescription;
        }

        public abstract void ExecuteTest();
    }
}

