using System.Text;
namespace VC71
{
    public sealed class CL : Microsoft.Build.CPPTasks.CL
    {
        protected override Encoding ResponseFileEncoding { get { return Encoding.Default; } }
    }
    public sealed class Link : Microsoft.Build.CPPTasks.Link
    {
        protected override Encoding ResponseFileEncoding { get { return Encoding.Default; } }
    }
    public sealed class LIB : Microsoft.Build.CPPTasks.LIB
    {
        protected override Encoding ResponseFileEncoding { get { return Encoding.Default; } }
        protected override string[] ReadTLogNames { get { return new[] { "link.read.*.tlog" }; } }
        protected override string[] WriteTLogNames { get { return new[] { "link.write.*.tlog" }; } }

        protected override string GenerateCommandLineCommands()
        {
            return "/LIB " + base.GenerateCommandLineCommands();
        }
    }
}
