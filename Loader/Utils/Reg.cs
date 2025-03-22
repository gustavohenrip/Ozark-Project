using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Win32;
using VMProtect;

namespace Loader {
    public class Reg {
        public static string GetValue(string key) {
            return Registry.GetValue(SDK.DecryptString("HKEY_CURRENT_USER\\Software\\Ozark"), key, null) as string;
        }

        public static void SetValue(string key, string value, RegistryValueKind type = RegistryValueKind.String) {
            Registry.SetValue(SDK.DecryptString("HKEY_CURRENT_USER\\Software\\Ozark"), key, value, type);
        }
    }
}
