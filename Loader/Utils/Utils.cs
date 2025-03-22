using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using VMProtect;

namespace Loader {
    class Utils {
        public static Thickness CalculateLabelPosition(Label label, Label result) {
            label.Measure(new Size(double.PositiveInfinity, double.PositiveInfinity));

            Thickness a = result.Margin;
            return new Thickness(label.DesiredSize.Width, a.Top, a.Right, a.Bottom);
        }

        public static byte[] ExtractResource(string filename) {
            System.Reflection.Assembly a = System.Reflection.Assembly.GetExecutingAssembly();
            using (Stream resFilestream = a.GetManifestResourceStream(filename)) {
                if (resFilestream == null) return null;
                byte[] ba = new byte[resFilestream.Length];
                resFilestream.Read(ba, 0, ba.Length);
                return ba;
            }
        }

        public static string GetTimeRemainingString(int Seconds_) {
            TimeSpan diff = TimeSpan.FromSeconds(Seconds_);
            int Years = diff.Days / 365;
            int Days = (diff.Days - (diff.Days / 365) * 365) - ((diff.Days - (diff.Days / 365) * 365) / 30) * 30;
            int Hours = diff.Hours;
            int Minutes = diff.Minutes;
            int Seconds = diff.Seconds;

            if (Years > 0) {
                return string.Format(SDK.DecryptString("{0}y {1}d {2}h {3}m {4}s"), Years, Days, Hours, Minutes, Seconds);
            } else {
                return string.Format(SDK.DecryptString("{0}d {1}h {2}m {3}s"), Days, Hours, Minutes, Seconds);
            }
        }

        public static string SendHttpRequest(string URL) {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(URL);
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();

            if (response.StatusCode == HttpStatusCode.OK) {
                Stream receiveStream = response.GetResponseStream();
                StreamReader readStream = null;

                if (string.IsNullOrWhiteSpace(response.CharacterSet))
                    readStream = new StreamReader(receiveStream);
                else
                    readStream = new StreamReader(receiveStream, Encoding.GetEncoding(response.CharacterSet));

                string data = readStream.ReadToEnd();

                response.Close();
                readStream.Close();

                return data;
            }

            return null;
        }

        public static string BytesToString(long byteCount) {
            string[] suf = { "B", "KB", "MB", "GB", "TB", "PB", "EB" }; //Longs run out around EB
            if (byteCount == 0)
                return "0" + suf[0];
            long bytes = Math.Abs(byteCount);
            int place = Convert.ToInt32(Math.Floor(Math.Log(bytes, 1024)));
            double num = Math.Round(bytes / Math.Pow(1024, place), 1);
            return (Math.Sign(byteCount) * num).ToString() + suf[place];
        }

        public static void SuccessMessage(string message) {
            MessageBox.Show(message, SDK.DecryptString("Ozark"), MessageBoxButton.OK, MessageBoxImage.Exclamation);
        }

        public static void ErrorMessage(string message) {
            MessageBox.Show(message, SDK.DecryptString("Ozark"), MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }
}
