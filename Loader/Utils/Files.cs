using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using VMProtect;

namespace Loader {
    class Files {
        public static void Initialize(List<string> cheatNames) {
            string ozark = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + SDK.DecryptString("\\Ozark");
            Directory.CreateDirectory(ozark);

            foreach (string name in cheatNames) {
                Directory.CreateDirectory(ozark + "\\" + name);
                Directory.CreateDirectory(ozark + "\\" + name + SDK.DecryptString("\\Themes"));
                Directory.CreateDirectory(ozark + "\\" + name + SDK.DecryptString("\\Translations"));

                if (name == SDK.DecryptString("Grand Theft Auto V") || name == SDK.DecryptString("Red Dead Redemption 2")) {
                    Directory.CreateDirectory(ozark + "\\" + name + SDK.DecryptString("\\Outfits"));
                    Directory.CreateDirectory(ozark + "\\" + name + SDK.DecryptString("\\ScriptHook"));
                }

                if (name == SDK.DecryptString("Grand Theft Auto V")) {
                    Directory.CreateDirectory(ozark + "\\" + name + SDK.DecryptString("\\Handling"));
                    Directory.CreateDirectory(ozark + "\\" + name + SDK.DecryptString("\\Menyoo"));
                    Directory.CreateDirectory(ozark + "\\" + name + SDK.DecryptString("\\LSC"));
                }
            }
        }

        public static string GetGameFolder(string cheatName) {
            return Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + SDK.DecryptString("\\Ozark\\") + cheatName + "\\";
        }
    }
}