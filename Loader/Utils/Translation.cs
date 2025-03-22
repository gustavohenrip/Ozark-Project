using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VMProtect;

namespace Loader {
    class Translation {
        public static bool IsChinese = false;
        public static Dictionary<string, string> Translations = new Dictionary<string, string>();

        public static void Initialize() {
            Translations[SDK.DecryptString("Enter your username...")] = "输入您的用户名...";
            Translations[SDK.DecryptString("Enter your password...")] = "输入您的密码...";
            Translations[SDK.DecryptString("Forgot")] = "忘记密码";
            Translations[SDK.DecryptString("Login")] = "登入";

            Translations[SDK.DecryptString("Game:")] = "游戏:";
            Translations[SDK.DecryptString("Subscription:")] = "订阅:";
            Translations[SDK.DecryptString("Cheat Status:")] = "作弊状态:";
            Translations[SDK.DecryptString("Game Status:")] = "游戏状态:";
            Translations[SDK.DecryptString("Version:")] = "版本:";
            Translations[SDK.DecryptString("Lifetime")] = "终身";
            Translations[SDK.DecryptString("Online")] = "在线";
            Translations[SDK.DecryptString("Offline")] = "离线";
            Translations[SDK.DecryptString("Querying...")] = "查询...";
            Translations[SDK.DecryptString("None")] = "无";
            Translations[SDK.DecryptString("View Changelog")] = "查看变更日志";
            Translations[SDK.DecryptString("View Resources")] = "查看资源";
            Translations[SDK.DecryptString("Load Cheat")] = "加载作弊";

            Translations[SDK.DecryptString("Grand Theft Auto V")] = "侠盗猎车手V";
            Translations[SDK.DecryptString("Red Dead Redemption 2")] = "荒野大镖客2";
            Translations[SDK.DecryptString("Sea of Thieves")] = "盗贼之海";
            Translations[SDK.DecryptString("Phasmophobia")] = "恐鬼症";

            Translations[SDK.DecryptString("Themes")] = "主题";
            Translations[SDK.DecryptString("Translations")] = "翻译";
            Translations[SDK.DecryptString("Bullet Modifiers")] = "公告修改";
            Translations[SDK.DecryptString("Vehicle LSC")] = "车辆LSC";
            Translations[SDK.DecryptString("Vehicle Handling")] = "车辆处理";

            Translations[SDK.DecryptString("Name:")] = "名称:";
            Translations[SDK.DecryptString("Author:")] = "作者:";
            Translations[SDK.DecryptString("Size:")] = "大小:";

            Translations[SDK.DecryptString("Download Translation")] = "下载翻译";
            Translations[SDK.DecryptString("Download Theme")] = "下载主题";
            Translations[SDK.DecryptString("Download Bullet Modifier")] = "下载子弹修改器";
            Translations[SDK.DecryptString("Download Vehicle LSC")] = "下载车辆LSC";
            Translations[SDK.DecryptString("Download Vehicle Handling")] = "下载车辆处理";

            Translations[SDK.DecryptString("Failed loading cheat!")] = "注入作弊失败!";
            Translations[SDK.DecryptString(" found, injecting in ")] = " 发现，注入 ";
            Translations[SDK.DecryptString("Waiting for ")] = "等待 ";
            Translations[SDK.DecryptString("Waiting...")] = "等待中...";
            Translations[SDK.DecryptString("Game closed while loading, please try again!")] = "注入时游戏已关闭，请重试!";

            Translations[SDK.DecryptString("If it takes a long time to download, you can use our manual files from the website. You'll need to use our launcher to load the cheat after downloading the client files to the location specified in the rar")] = "如果下载时间较长，则可以从网站上使用我们的手册文件。将客户端文件下载到RDR中指定的位置后，您再使用我们的启动器加载作弊文件";
        }

        public static string GetString(string str) {
            if (IsChinese && Translations.ContainsKey(str)) {
                return Translations[str];
            }

            return str;
        }
    }
}
