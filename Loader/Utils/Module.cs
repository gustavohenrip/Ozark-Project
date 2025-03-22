using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using VMProtect;

namespace Loader {
    class Module {
        [DllImport("kernel32.dll")]
        public static extern IntPtr LoadLibrary(string Module);

        [DllImport("kernel32.dll")]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string ProcName);

        public delegate IntPtr Delegate1(string A);
        public static Delegate1 AAAAAAAA = (Delegate1)null;
        public static Delegate1 BBBBBBBB = (Delegate1)null;
        public static Delegate1 CCCCCCCC = (Delegate1)null;
        public static Delegate1 DDDDDDDD = (Delegate1)null;

        public delegate void Delegate2(IntPtr A);
        public static Delegate2 EEEEEEEE = (Delegate2)null;

        public delegate bool Delegate3(string A, string B);
        public static Delegate3 FFFFFFFF = (Delegate3)null;

        public static bool Initialize() {
            string AppDataPath = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
            string ModuleName = SDK.DecryptString("BugTrace01.dll");

            if (File.Exists(AppDataPath + "\\" + ModuleName)) {
                File.Delete(AppDataPath + "\\" + ModuleName);
            }

            File.WriteAllBytes(AppDataPath + "\\" + ModuleName, Utils.ExtractResource(SDK.DecryptString("Loader.CheatLauncherBackend.vmp.dll")));

            IntPtr module = LoadLibrary(AppDataPath + "\\" + ModuleName);
            // IntPtr module = LoadLibrary("CheatLauncherBackend.dll");
            if (module.ToInt64() != 0) {
                AAAAAAAA = (Delegate1)Marshal.GetDelegateForFunctionPointer(GetProcAddress(module, SDK.DecryptString("AAAAAAAA")), typeof(Delegate1));
                BBBBBBBB = (Delegate1)Marshal.GetDelegateForFunctionPointer(GetProcAddress(module, SDK.DecryptString("BBBBBBBB")), typeof(Delegate1));
                CCCCCCCC = (Delegate1)Marshal.GetDelegateForFunctionPointer(GetProcAddress(module, SDK.DecryptString("CCCCCCCC")), typeof(Delegate1));
                DDDDDDDD = (Delegate1)Marshal.GetDelegateForFunctionPointer(GetProcAddress(module, SDK.DecryptString("DDDDDDDD")), typeof(Delegate1));
                EEEEEEEE = (Delegate2)Marshal.GetDelegateForFunctionPointer(GetProcAddress(module, SDK.DecryptString("EEEEEEEE")), typeof(Delegate2));
                FFFFFFFF = (Delegate3)Marshal.GetDelegateForFunctionPointer(GetProcAddress(module, SDK.DecryptString("FFFFFFFF")), typeof(Delegate3));
                return true;
            }

            return false;
        }
    }
}
