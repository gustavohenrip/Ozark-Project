using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using VMProtect;

namespace Loader {
    /// <summary>
    /// Interaction logic for WaitingWindow.xaml
    /// </summary>
    public partial class WaitingWindow : Window {
        public Classes.Cheat SelectedCheat;
        private Thread WorkerThread;
        private int SecondsRemaining = 0;

        public WaitingWindow() {
            InitializeComponent();
        }

        private void WaitingWindow_Loaded(object sender, RoutedEventArgs e) {
            Title = Translation.GetString(SDK.DecryptString("Waiting..."));
            WaitingLabel.Content = Translation.GetString(SDK.DecryptString("Waiting for ")) + SelectedCheat.Process;
            SecondsRemaining = SelectedCheat.InjectionTimeout + 1;

            WorkerThread = new Thread(() => {
                while (true) {
                    Application.Current.Dispatcher.Invoke((Action)(() => {
                        if (Process.GetProcessesByName(SelectedCheat.Process.Substring(0, SelectedCheat.Process.Length - 4)).Length > 0) {
                            Application.Current.Dispatcher.Invoke((Action)(() => {
                                SecondsRemaining--;
                                WaitingLabel.Content = SelectedCheat.Process + Translation.GetString(SDK.DecryptString(" found, injecting in ")) + SecondsRemaining + Translation.GetString(SDK.DecryptString("s..."));
                            }));

                            if (SecondsRemaining <= 0) {
                                string clientPath = Files.GetGameFolder(SelectedCheat.Name) + SDK.DecryptString("client.dll");
                                if (SelectedCheat.CheatID == Classes.eCheats.CHEAT_RED_DEAD_REDEMPTION_2) {
                                    clientPath = Files.GetGameFolder(SelectedCheat.Name) + SDK.DecryptString("bypass.dll");
                                }

                                if (Module.FFFFFFFF(clientPath, SelectedCheat.Process)) {
                                    Environment.Exit(0);
                                } else {
                                    Utils.ErrorMessage(Translation.GetString(SDK.DecryptString("Failed loading cheat!")));
                                    Close();
                                    MainWindow.ThisWindow.Show();
                                    WorkerThread.Abort();
                                }
                            }
                        } else {
                            // game closed while loading
                            if (SecondsRemaining < SelectedCheat.InjectionTimeout) {
                                Utils.ErrorMessage(Translation.GetString(SDK.DecryptString("Game closed while loading, please try again!")));
                                Close();
                                MainWindow.ThisWindow.Show();
                                WorkerThread.Abort();
                            }
                        }
                    }));

                    Thread.Sleep(1000);
                }
            });

            WorkerThread.Start();
        }

        private void BannerRectangle_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left) {
                DragMove();
            }
        }

        private void CloseButtonImage_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left) {
                WorkerThread.Abort();
                Close();
                MainWindow.ThisWindow.Show();
            }
        }

        private void MinimizeButtonImage_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left) {
                WindowState = WindowState.Minimized;
            }
        }

        private void HeaderImage_MouseEnter(object sender, MouseEventArgs e) {
            Image image = sender as Image;
            image.Cursor = Cursors.Hand;
        }

        private void HeaderImage_MouseLeave(object sender, MouseEventArgs e) {
            Image image = sender as Image;
            image.Cursor = Cursors.None;
        }

        private void URLLabel_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left) {
                System.Diagnostics.Process.Start(Translation.GetString(SDK.DecryptString("https://ozark.gg")));
            }
        }
    }
}