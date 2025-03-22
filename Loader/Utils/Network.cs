using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Imaging;
using System.IO;
using VMProtect;

namespace Loader {
    enum ePackets : int {
        PACKET_AUTHENTICATE = 1,
        PACKET_DOWNLOAD_CLIENT = 2,
        PACKET_DOWNLOAD_RDR2_BYPASS = 3,
        PACKET_GET_LAUNCHER_VERSION = 420
    };

    public struct CheatInfoResponse {
        public int id;
        public string name;
        public bool online;
        public string process;
        public int inject_timeout;
        public int current_version;
        public int time;
    };

    class Network {
        static int ThisVersion = 5;
        public static string LastErrorString = "";

        public static bool CheckLauncherVersion() {
            HttpClient httpClient = new HttpClient();
            httpClient.DefaultRequestHeaders.Add(SDK.DecryptString("User-Agent"), SDK.DecryptString("Envious Poser MIKE"));
            httpClient.Timeout = TimeSpan.FromMinutes(30);

            JObject jObject = new JObject();
            jObject[SDK.DecryptString("id")] = ePackets.PACKET_GET_LAUNCHER_VERSION.GetHashCode();

            HttpResponseMessage response = httpClient.PostAsync(SDK.DecryptString("https://ozarkengine.cc/api.php"), new StringContent(jObject.ToString(), Encoding.UTF8, SDK.DecryptString("application/json"))).Result;

            string responseBody = response.Content.ReadAsStringAsync().Result;
            if (responseBody.Length > 0) {
                IntPtr allocation = Module.CCCCCCCC(responseBody);
                string decryptedResponseBody = Marshal.PtrToStringAnsi(allocation);
                Module.EEEEEEEE(allocation);

                if (decryptedResponseBody.Length > 0) {
                    jObject = JObject.Parse(decryptedResponseBody);
                    if (jObject.Count > 0) {
                        if (jObject.Value<string>(SDK.DecryptString("type")) == SDK.DecryptString("error")) {
                            // bad
                            LastErrorString = jObject.Value<string>(SDK.DecryptString("message"));
                            return false;
                        }

                        int latestVersion = -1;
                        if (int.TryParse(jObject.Value<string>(SDK.DecryptString("message")), out latestVersion)) {
                            if (latestVersion > ThisVersion) {
                                // outdated
                                LastErrorString = SDK.DecryptString("Launcher is outdated. Please download the latest from our website, ozark.gg.");
                                return false;
                            }

                            return true;
                        }
                    }
                }
            }

            return false;
        }
        
        public static string GetChangelog(string cheat) {
            HttpResponseMessage response = new HttpClient().GetAsync(SDK.DecryptString("https://ozark.gg/server/cheats/") + cheat.Replace(" ", "%20") + SDK.DecryptString("/changelog.txt")).Result;

            if (response.Content.ReadAsStringAsync().Result.Contains(SDK.DecryptString("<html>"))) return null;
            return response.Content.ReadAsStringAsync().Result;
        }

        public static string GetResources(string cheat) {
            HttpResponseMessage response = new HttpClient().GetAsync(SDK.DecryptString("https://ozark.gg/server/cheats/") + cheat.Replace(" ", "%20") + SDK.DecryptString("/resources.json")).Result;

            if (response.Content.ReadAsStringAsync().Result.Contains(SDK.DecryptString("<html>"))) return null;
            return response.Content.ReadAsStringAsync().Result;
        }

        public static string GetResource(string cheat, string type, string folder, string file) {
            HttpResponseMessage response = new HttpClient().GetAsync(SDK.DecryptString("https://ozark.gg/server/cheats/") + cheat.Replace(" ", "%20") + "/" + type + "/" + folder + "/" + file).Result;

            if (response.Content.ReadAsStringAsync().Result.Contains(SDK.DecryptString("<html>"))) return null;
            return response.Content.ReadAsStringAsync().Result;
        }

        public static byte[][] GetAssets() {
            HttpResponseMessage mainYtd = new HttpClient().GetAsync(SDK.DecryptString("https://ozark.gg/server/cheats/" + "Grand Theft Auto V".Replace(" ", "%20") + "/Ozark.ytd")).Result;
            if (mainYtd.Content.ReadAsStringAsync().Result.Contains(SDK.DecryptString("<html>"))) return null;

            HttpResponseMessage blipYtd = new HttpClient().GetAsync(SDK.DecryptString("https://ozark.gg/server/cheats/" + "Grand Theft Auto V".Replace(" ", "%20") + "/OzarkBlips.ytd")).Result;
            if (blipYtd.Content.ReadAsStringAsync().Result.Contains(SDK.DecryptString("<html>"))) return null;

            byte[][] data = new byte[2][];
            data[0] = mainYtd.Content.ReadAsByteArrayAsync().Result;
            data[1] = blipYtd.Content.ReadAsByteArrayAsync().Result;
            return data;
        }

        public static bool Authenticate(string username, string password) {
            HttpClient httpClient = new HttpClient();
            httpClient.DefaultRequestHeaders.Add(SDK.DecryptString("User-Agent"), SDK.DecryptString("Envious Poser MIKE"));
            httpClient.Timeout = TimeSpan.FromMinutes(30);

            IntPtr usernameAllocation = Module.AAAAAAAA(username);
            IntPtr passwordAllocation = Module.DDDDDDDD(password);

            JObject jObject = new JObject();
            jObject[SDK.DecryptString("id")] = ePackets.PACKET_AUTHENTICATE.GetHashCode();
            jObject[SDK.DecryptString("username")] = Marshal.PtrToStringAnsi(usernameAllocation);
            jObject[SDK.DecryptString("password")] = Marshal.PtrToStringAnsi(passwordAllocation);

            Module.EEEEEEEE(usernameAllocation);
            Module.EEEEEEEE(passwordAllocation);

            HttpResponseMessage response = httpClient.PostAsync(SDK.DecryptString("https://ozarkengine.cc/api.php"), new StringContent(jObject.ToString(), Encoding.UTF8, SDK.DecryptString("application/json"))).Result;
            
            string responseBody = response.Content.ReadAsStringAsync().Result;
            if (responseBody.Length > 0) {
                IntPtr allocation = Module.CCCCCCCC(responseBody);
                string decryptedResponseBody = Marshal.PtrToStringAnsi(allocation);
                Module.EEEEEEEE(allocation);

                if (decryptedResponseBody.Length > 0) {
                    jObject = JObject.Parse(decryptedResponseBody);
                    if (jObject.Count > 0) {
                        if (jObject.Value<string>(SDK.DecryptString("type")) == SDK.DecryptString("error")) {
                            // bad
                            LastErrorString = jObject.Value<string>(SDK.DecryptString("message"));
                            return false;
                        }

                        // handle
                        int counter = 0;

                        List<string> cheatNames = new List<string>();
                        List<CheatInfoResponse> cheatInfoResponse = JsonConvert.DeserializeObject<List<CheatInfoResponse>>(jObject.Value<JArray>(SDK.DecryptString("message")).ToString());
                        foreach (var cheat in cheatInfoResponse) {
                            Classes.Cheat cheats = new Classes.Cheat();
                            cheats.ThumbnailImage = new BitmapImage(new Uri(SDK.DecryptString("https://ozark.gg/server/cheats/") + cheat.name.Replace(" ", "%20") + SDK.DecryptString("/thumb.jpg")));
                            cheats.Image = new BitmapImage(new Uri(SDK.DecryptString("https://ozark.gg/server/cheats/") + cheat.name.Replace(" ", "%20") + SDK.DecryptString("/selected.jpg")));
                            cheats.Name = cheat.name;
                            cheats.Version = cheat.current_version;
                            cheats.Index = counter++;
                            cheats.SecondsLeft = cheat.time;
                            cheats.CheatID = (Classes.eCheats)cheat.id;
                            cheats.Process = cheat.process;
                            cheats.Online = cheat.online;
                            cheats.InjectionTimeout = cheat.inject_timeout;

                            MainWindow.Cheats.Add(cheats);
                            cheatNames.Add(cheats.Name);
                        }

                        Files.Initialize(cheatNames);

                        if (!File.Exists(Files.GetGameFolder("Grand Theft Auto V") + "Ozark.ytd")) {
                            byte[][] assets = GetAssets();
                            File.WriteAllBytes(Files.GetGameFolder("Grand Theft Auto V") + "Ozark.ytd", assets[0]);
                            File.WriteAllBytes(Files.GetGameFolder("Grand Theft Auto V") + "OzarkBlips.ytd", assets[1]);
                        }

                        return true;
                    }
                }
            }

            return false;
        }

        public static bool DownloadCheat(string username, string password, Classes.Cheat cheat) {
            HttpClient httpClient = new HttpClient();
            httpClient.DefaultRequestHeaders.Add(SDK.DecryptString("User-Agent"), SDK.DecryptString("Envious Poser MIKE"));
            httpClient.Timeout = TimeSpan.FromMinutes(30);

            IntPtr usernameAllocation = Module.AAAAAAAA(username);
            IntPtr passwordAllocation = Module.DDDDDDDD(password);

            JObject jObject = new JObject();
            jObject[SDK.DecryptString("id")] = ePackets.PACKET_DOWNLOAD_CLIENT.GetHashCode();
            jObject[SDK.DecryptString("username")] = Marshal.PtrToStringAnsi(usernameAllocation);
            jObject[SDK.DecryptString("password")] = Marshal.PtrToStringAnsi(passwordAllocation);
            jObject[SDK.DecryptString("cheat_id")] = cheat.CheatID.GetHashCode();

            Module.EEEEEEEE(usernameAllocation);
            Module.EEEEEEEE(passwordAllocation);

            HttpResponseMessage response = httpClient.PostAsync("https://ozarkengine.cc/api.php", new StringContent(jObject.ToString(), Encoding.UTF8, SDK.DecryptString("application/json"))).Result;

            string responseBody = response.Content.ReadAsStringAsync().Result;
            if (responseBody.Length > 0) {
                if (responseBody.Substring(0, 4) == SDK.DecryptString("1337")) {
                    File.WriteAllBytes(Files.GetGameFolder(cheat.Name) + SDK.DecryptString("client.dll"), response.Content.ReadAsByteArrayAsync().Result.Skip(4).ToArray()); 
                    return true;
                }

                IntPtr allocation = Module.CCCCCCCC(responseBody);
                string decryptedResponseBody = Marshal.PtrToStringAnsi(allocation);
                Module.EEEEEEEE(allocation);

                if (decryptedResponseBody.Length > 0) {
                    jObject = JObject.Parse(decryptedResponseBody);
                    if (jObject.Count > 0) {
                        LastErrorString = jObject.Value<string>(SDK.DecryptString("message"));
                    }
                }
            }

            return false;
        }

        public static bool DownloadBypass(string username, string password) {
            HttpClient httpClient = new HttpClient();
            httpClient.DefaultRequestHeaders.Add(SDK.DecryptString("User-Agent"), SDK.DecryptString("Envious Poser MIKE"));
            httpClient.Timeout = TimeSpan.FromMinutes(30);

            IntPtr usernameAllocation = Module.AAAAAAAA(username);
            IntPtr passwordAllocation = Module.DDDDDDDD(password);

            JObject jObject = new JObject();
            jObject[SDK.DecryptString("id")] = ePackets.PACKET_DOWNLOAD_RDR2_BYPASS.GetHashCode();
            jObject[SDK.DecryptString("username")] = Marshal.PtrToStringAnsi(usernameAllocation);
            jObject[SDK.DecryptString("password")] = Marshal.PtrToStringAnsi(passwordAllocation);

            Module.EEEEEEEE(usernameAllocation);
            Module.EEEEEEEE(passwordAllocation);

            HttpResponseMessage response = httpClient.PostAsync("https://ozarkengine.cc/api.php", new StringContent(jObject.ToString(), Encoding.UTF8, SDK.DecryptString("application/json"))).Result;

            string responseBody = response.Content.ReadAsStringAsync().Result;
            if (responseBody.Length > 0) {
                if (responseBody.Substring(0, 4) == SDK.DecryptString("1337")) {
                    File.WriteAllBytes(Files.GetGameFolder("Red Dead Redemption 2") + SDK.DecryptString("bypass.dll"), response.Content.ReadAsByteArrayAsync().Result.Skip(4).ToArray());
                    return true;
                }

                IntPtr allocation = Module.CCCCCCCC(responseBody);
                string decryptedResponseBody = Marshal.PtrToStringAnsi(allocation);
                Module.EEEEEEEE(allocation);

                if (decryptedResponseBody.Length > 0) {
                    jObject = JObject.Parse(decryptedResponseBody);
                    if (jObject.Count > 0) {
                        LastErrorString = jObject.Value<string>(SDK.DecryptString("message"));
                    }
                }
            } else {
                MessageBox.Show("BAD LENGTH...");
            }

            return false;
        }
    }
}