using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using VMProtect;

namespace Loader {
    /// <summary>
    /// Interaction logic for LoginWindow.xaml
    /// </summary>
    public partial class LoginWindow : Window {
        public static string Username = "";
        public static string Password = "";

        public LoginWindow() {
            InitializeComponent();

            ServicePointManager.Expect100Continue = true;
            ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls12;

            if (!Module.Initialize()) {
                Environment.Exit(0);
            }

            Translation.Initialize();

            string username = Reg.GetValue(SDK.DecryptString("Username"));
            if (username != null) {
                IntPtr allocation = Module.BBBBBBBB(username);
                UsernameTextBox.Text = Marshal.PtrToStringAnsi(allocation); // decrypt
                UsernamePlaceholderTextBox.Visibility = Visibility.Hidden;

                Module.EEEEEEEE(allocation);
            }

            string password = Reg.GetValue(SDK.DecryptString("Password"));
            if (password != null) {
                IntPtr allocation = Module.BBBBBBBB(password);
                PasswordTextBox.Password = Marshal.PtrToStringAnsi(allocation); // decrypt
                PasswordPlaceholderTextBox.Visibility = Visibility.Hidden;

                Module.EEEEEEEE(allocation);
            }
        }

        private void UpdateTranslationLabels() {
            UsernamePlaceholderTextBox.Text = Translation.GetString(SDK.DecryptString("Enter your username..."));
            PasswordPlaceholderTextBox.Text = Translation.GetString(SDK.DecryptString("Enter your password..."));
            ForgotPasswordButton.Content = Translation.GetString(SDK.DecryptString("Forgot"));
            LoginButton.Content = Translation.GetString(SDK.DecryptString("Login"));
        }

        private void BannerRectangle_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left) {
                DragMove();
            }
        }

        private void CloseButtonImage_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left) {
                Environment.Exit(0);
            }
        }

        private void MinimizeButtonImage_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left) {
                WindowState = WindowState.Minimized;
            }
        }

        private void UsernameTextBox_GotFocus(object sender, RoutedEventArgs e) {
            UsernamePlaceholderTextBox.Visibility = Visibility.Hidden;
        }

        private void UsernameTextBox_LostFocus(object sender, RoutedEventArgs e) {
            if (UsernameTextBox.Text.Length == 0) {
                UsernamePlaceholderTextBox.Visibility = Visibility.Visible;
            }
        }

        private void PasswordTextBox_GotFocus(object sender, RoutedEventArgs e) {
            PasswordPlaceholderTextBox.Visibility = Visibility.Hidden;
        }

        private void PasswordTextBox_LostFocus(object sender, RoutedEventArgs e) {
            if (PasswordTextBox.Password.Length == 0) {
                PasswordPlaceholderTextBox.Visibility = Visibility.Visible;
            }
        }

        private void CloseButtonImage_MouseEnter(object sender, MouseEventArgs e) {
            CloseButtonImage.Cursor = Cursors.Hand;
        }

        private void CloseButtonImage_MouseLeave(object sender, MouseEventArgs e) {
            CloseButtonImage.Cursor = Cursors.None;
        }

        private void MinimizeButtonImage_MouseEnter(object sender, MouseEventArgs e) {
            MinimizeButtonImage.Cursor = Cursors.Hand;
        }

        private void MinimizeButtonImage_MouseLeave(object sender, MouseEventArgs e) {
            MinimizeButtonImage.Cursor = Cursors.None;
        }

        private void USALanguageImage_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left) {
                ChinaLanguageImage.Opacity = 0.1;
                USALanguageImage.Opacity = 1.0;

                // Update translations for the login page.
                Translation.IsChinese = false;
                UpdateTranslationLabels();
            }
        }

        private void ChinaLanguageImage_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left) {
                ChinaLanguageImage.Opacity = 1.0;
                USALanguageImage.Opacity = 0.1;

                // Update translations for the login page.
                // Translation will automatically apply once on main form
                Translation.IsChinese = true;
                UpdateTranslationLabels();
            }
        }

        private void USALanguageImage_MouseEnter(object sender, MouseEventArgs e) {
            USALanguageImage.Cursor = Cursors.Hand;
            USALanguageImage.Opacity = 1.0;
        }

        private void USALanguageImage_MouseLeave(object sender, MouseEventArgs e) {
            USALanguageImage.Cursor = Cursors.None;

            if (Translation.IsChinese) {
                USALanguageImage.Opacity = 0.1;
            }
        }

        private void ChinaLanguageImage_MouseEnter(object sender, MouseEventArgs e) {
            ChinaLanguageImage.Cursor = Cursors.Hand;
            ChinaLanguageImage.Opacity = 1.0;
        }

        private void ChinaLanguageImage_MouseLeave(object sender, MouseEventArgs e) {
            ChinaLanguageImage.Cursor = Cursors.None;

            if (!Translation.IsChinese) {
                ChinaLanguageImage.Opacity = 0.1;
            }
        }

        private void LoginButton_Click(object sender, RoutedEventArgs e) {
            IntPtr usernameAllocation = Module.AAAAAAAA(UsernameTextBox.Text);
            IntPtr passwordAllocation = Module.AAAAAAAA(PasswordTextBox.Password);

            Reg.SetValue(SDK.DecryptString("Username"), Marshal.PtrToStringAnsi(usernameAllocation));
            Reg.SetValue(SDK.DecryptString("Password"), Marshal.PtrToStringAnsi(passwordAllocation));

            Module.EEEEEEEE(usernameAllocation);
            Module.EEEEEEEE(passwordAllocation);

            // check this version
            if (!Network.CheckLauncherVersion()) {
                MessageBox.Show(Network.LastErrorString);
                Environment.Exit(0);
            }

            // authenticate
            if (!Network.Authenticate(UsernameTextBox.Text, PasswordTextBox.Password)) {
                MessageBox.Show(Network.LastErrorString);
                // Environment.Exit(0);
            } else {
                Username = UsernameTextBox.Text;
                Password = PasswordTextBox.Password;

                Hide();
                new MainWindow().Show();
            }
        }

        private void ForgotPasswordButton_Click(object sender, RoutedEventArgs e) {
            System.Diagnostics.Process.Start(SDK.DecryptString("https://ozark.gg/forgot.html"));
        }
    }
}
