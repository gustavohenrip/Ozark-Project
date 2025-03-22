using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.IO;
using System.Diagnostics;
using VMProtect;

namespace Loader {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window {
        public static List<Classes.Cheat> Cheats = new List<Classes.Cheat>();
        public static Dictionary<int, Tuple<BitmapImage, BitmapImage>> CheatImages = new Dictionary<int, Tuple<BitmapImage, BitmapImage>>();
        public static int SelectedCheat = 0;
        public static Window ThisWindow;

        public MainWindow() {
            InitializeComponent();

            ThisWindow = this;

            int counter = 0;
            foreach (var cheat in Cheats) {
                cheat.BorderHandle = CreateListItem(counter, Translation.GetString(cheat.Name), "V" + cheat.Version.ToString(), cheat.ThumbnailImage, CheatPanelBorder_MouseEnter, 
                    CheatPanelBorder_MouseLeave, CheatPanelBorder_MouseDown);

                CheatPanel.Children.Add(cheat.BorderHandle);
                counter++;
            }

            // start thread to monitor seconds left
            new Thread(() => {
                while (true) {
                    Application.Current.Dispatcher.Invoke((Action)(() => {
                        foreach (var c in Cheats) {
                            if (c.SecondsLeft != int.MaxValue && c.SecondsLeft != 0) {
                                c.SecondsLeft--;
                            }
                        }

                        UpdateLabels();
                    }));

                    Thread.Sleep(1000);
                }
            }).Start();

            // start thread to check game statuses
            new Thread(() => {
                // Rockstar
                string websiteData = Utils.SendHttpRequest(SDK.DecryptString("https://support.rockstargames.com/services/status.json?tz=Europe/London"));
                if (websiteData.Length > 0) {
                    JObject Obj = JObject.Parse(websiteData);
                    if (Obj.Count > 0) {
                        JArray Status = Obj.Value<JArray>(SDK.DecryptString("statuses"));
                        if (Status.Count > 0) {
                            foreach (JObject Item in Status) {
                                if (Item.GetValue(SDK.DecryptString("name")).ToString() == SDK.DecryptString("Grand Theft Auto Online")) {
                                    JArray Platforms = Item.Value<JArray>(SDK.DecryptString("services_platforms"));
                                    foreach (JObject PlatformItem in Platforms) {
                                        if (PlatformItem.GetValue(SDK.DecryptString("name")).ToString() == SDK.DecryptString("PC")) {
                                            GetCheatFromName(SDK.DecryptString("Grand Theft Auto V")).GameOnlineQueried = true;
                                            GetCheatFromName(SDK.DecryptString("Grand Theft Auto V")).GameOnline = ((JObject)PlatformItem.GetValue(SDK.DecryptString("service_status"))).GetValue(SDK.DecryptString("status")).ToString() == SDK.DecryptString("UP");
                                        }
                                    }
                                } else if (Item.GetValue(SDK.DecryptString("name")).ToString() == SDK.DecryptString("Red Dead Online")) {
                                    JArray Platforms = Item.Value<JArray>(SDK.DecryptString("services_platforms"));
                                    foreach (JObject PlatformItem in Platforms) {
                                        if (PlatformItem.GetValue(SDK.DecryptString("name")).ToString() == SDK.DecryptString("PC")) {
                                            GetCheatFromName(SDK.DecryptString("Red Dead Redemption 2")).GameOnlineQueried = true;
                                            GetCheatFromName(SDK.DecryptString("Red Dead Redemption 2")).GameOnline = ((JObject)PlatformItem.GetValue(SDK.DecryptString("service_status"))).GetValue(SDK.DecryptString("status")).ToString() == SDK.DecryptString("UP");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }).Start();

            // update current cheat (index 0) for labels
            UpdateLabels();

            // update element translations that are static in xaml
            URLLabel.Content = Translation.GetString(URLLabel.Content as string);
            ViewChangelogButton.Content = Translation.GetString(ViewChangelogButton.Content as string);
            ViewResourcesButton.Content = Translation.GetString(ViewResourcesButton.Content as string);
            LoadCheatButton.Content = Translation.GetString(LoadCheatButton.Content as string);
            GameLabel.Content = Translation.GetString(GameLabel.Content as string);
            VersionLabel.Content = Translation.GetString(VersionLabel.Content as string);
            GameStatusLabel.Content = Translation.GetString(GameStatusLabel.Content as string);
            CheatStatusLabel.Content = Translation.GetString(CheatStatusLabel.Content as string);
            SubscriptionLabel.Content = Translation.GetString(SubscriptionLabel.Content as string);
            UsernameLabel.Content = LoginWindow.Username;
        }

        Border CreateListItem(int counter, string name, string version, BitmapImage thumbnail, MouseEventHandler mouseEnter, MouseEventHandler mouseLeave, MouseButtonEventHandler mouseDown) {
            Border rectangleBorder = new Border();
            rectangleBorder.BorderThickness = new Thickness(1);
            rectangleBorder.CornerRadius = new CornerRadius(6);
            rectangleBorder.Height = 50;
            rectangleBorder.Opacity = 0.9;

            if (counter > 0) {
                rectangleBorder.Background = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#252729"));
            } else rectangleBorder.Background = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#424447"));

            Label cheatName = new Label();
            cheatName.Content = name;
            cheatName.Foreground = new SolidColorBrush(Colors.White);
            cheatName.FontFamily = new FontFamily(SDK.DecryptString("Franklin Gothic Medium"));
            cheatName.FontSize = 13;
            cheatName.FontWeight = FontWeights.Bold;

            Label cheatVersion = new Label();
            cheatVersion.Content = version;
            cheatVersion.Foreground = new SolidColorBrush(Colors.Gray);
            cheatVersion.FontFamily = new FontFamily(SDK.DecryptString("Franklin Gothic Medium"));
            cheatVersion.FontSize = 13;
            cheatVersion.Padding = new Thickness(0, 5, 5, 5);

            StackPanel stackPanel = new StackPanel();
            stackPanel.Orientation = Orientation.Horizontal;
            stackPanel.HorizontalAlignment = HorizontalAlignment.Left;
            stackPanel.VerticalAlignment = VerticalAlignment.Center;
            stackPanel.Margin = new Thickness(95, 0, 0, 0);
            if (counter > 0) stackPanel.Opacity = 0.6; // 0.6 if not selected
            stackPanel.Children.Add(cheatName);
            stackPanel.Children.Add(cheatVersion);

            ImageBrush imageBrush = new ImageBrush();
            imageBrush.Stretch = Stretch.Fill;
            imageBrush.ImageSource = thumbnail;
            if (counter > 0) imageBrush.Opacity = 0.6; // 0.6 if not selected

            Border imageBorder = new Border();
            imageBorder.CornerRadius = new CornerRadius(6);
            imageBorder.Height = 50;
            imageBorder.Width = 85;
            imageBorder.HorizontalAlignment = HorizontalAlignment.Left;
            imageBorder.Margin = new Thickness(0);
            imageBorder.Background = imageBrush;

            Grid grid = new Grid();
            grid.Children.Add(rectangleBorder);
            grid.Children.Add(imageBorder);
            grid.Children.Add(stackPanel);

            Border handle = new Border();
            if (counter > 0) handle.Margin = new Thickness(0, 5, 0, 0);
            handle.Child = grid;
            handle.MouseEnter += mouseEnter;
            handle.MouseLeave += mouseLeave;
            handle.MouseDown += mouseDown;

            return handle;
        }

        Classes.Cheat GetCheatFromBorder(Border handle) {
            foreach (var cheat in Cheats) {
                if (cheat.BorderHandle == handle) {
                    return cheat;
                }
            }

            return null;
        }

        Classes.Cheat GetCheatFromName(string name) {
            foreach (var cheat in Cheats) {
                if (cheat.Name == name) {
                    return cheat;
                }
            }

            return null;
        }

        private void UpdateLabels() {
            // update labels
            SelectedCheatGameName.Content = Translation.GetString(Cheats[SelectedCheat].Name);
            SelectedCheatVersion.Content = "V" + Cheats[SelectedCheat].Version.ToString();
            SelectedCheatImage.Source = Cheats[SelectedCheat].Image;

            if (Cheats[SelectedCheat].GameOnlineQueried) {
                if (Cheats[SelectedCheat].GameOnline) {
                    SelectedCheatGameStatus.Content = Translation.GetString(SDK.DecryptString("Online"));
                    SelectedCheatGameStatus.Foreground = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#0e9414"));
                } else {
                    SelectedCheatGameStatus.Content = Translation.GetString(SDK.DecryptString("Offline"));
                    SelectedCheatGameStatus.Foreground = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#940e0e"));
                }
            } else {
                SelectedCheatGameStatus.Content = Translation.GetString(SDK.DecryptString("Querying..."));
                SelectedCheatGameStatus.Foreground = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#94820e"));
            }

            if (Cheats[SelectedCheat].SecondsLeft == int.MaxValue) {
                SelectedCheatSubscription.Content = Translation.GetString(SDK.DecryptString("Lifetime"));
                SelectedCheatSubscription.Foreground = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#0e9414"));
            } else {
                if (Cheats[SelectedCheat].SecondsLeft == 0) {
                    SelectedCheatSubscription.Content = Translation.GetString(SDK.DecryptString("None"));
                    SelectedCheatSubscription.Foreground = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#940e0e"));
                } else {
                    SelectedCheatSubscription.Content = Utils.GetTimeRemainingString(Cheats[SelectedCheat].SecondsLeft);
                    SelectedCheatSubscription.Foreground = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#DDDDDD"));
                }
            }

            if (Cheats[SelectedCheat].Online) {
                SelectedCheatStatus.Content = Translation.GetString(SDK.DecryptString("Online"));
                SelectedCheatStatus.Foreground = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#0e9414"));
            } else {
                SelectedCheatStatus.Content = Translation.GetString(SDK.DecryptString("Offline"));
                SelectedCheatStatus.Foreground = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#940e0e"));
            }

            SelectedCheatGameName.Margin = Utils.CalculateLabelPosition(GameLabel, SelectedCheatGameName);
            SelectedCheatVersion.Margin = Utils.CalculateLabelPosition(VersionLabel, SelectedCheatVersion);
            SelectedCheatGameStatus.Margin = Utils.CalculateLabelPosition(GameStatusLabel, SelectedCheatGameStatus);
            SelectedCheatStatus.Margin = Utils.CalculateLabelPosition(CheatStatusLabel, SelectedCheatStatus);
            SelectedCheatSubscription.Margin = Utils.CalculateLabelPosition(SubscriptionLabel, SelectedCheatSubscription);

            LoadCheatButton.IsEnabled = Cheats[SelectedCheat].SecondsLeft != 0 && Cheats[SelectedCheat].Online;
        }

        private void CheatPanelBorder_MouseDown(object sender, MouseButtonEventArgs e) {
            Classes.Cheat cheat = GetCheatFromBorder(sender as Border);
            if (cheat != null) {
                // set selected cheat index for buttons
                SelectedCheat = cheat.Index;

                UpdateLabels();

                // update each event by emulating a mouseleave event on each border
                foreach (var child in CheatPanel.Children) {
                    CheatPanelBorder_MouseLeave(child as Border, null);
                }
            }
        }

        private void CheatPanelBorder_MouseEnter(object sender, MouseEventArgs e) {
            Classes.Cheat cheat = GetCheatFromBorder(sender as Border);
            if (cheat != null) {
                cheat.BorderHandle.Cursor = Cursors.Hand;

                Grid grid = cheat.BorderHandle.Child as Grid;

                Border rectangleBorder = grid.Children[0] as Border;
                rectangleBorder.Background = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#424447"));

                Border imageBorder = grid.Children[1] as Border;
                imageBorder.Background.Opacity = 1.0;

                StackPanel stackPanel = grid.Children[2] as StackPanel;
                stackPanel.Opacity = 1.0;
            }
        }

        private void CheatPanelBorder_MouseLeave(object sender, MouseEventArgs e) {
            Classes.Cheat cheat = GetCheatFromBorder(sender as Border);
            if (cheat != null) {
                if (e != null) cheat.BorderHandle.Cursor = Cursors.None;

                if (cheat.Index != SelectedCheat) {
                    Grid grid = cheat.BorderHandle.Child as Grid;

                    Border rectangleBorder = grid.Children[0] as Border;
                    rectangleBorder.Background = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#252729"));

                    Border imageBorder = grid.Children[1] as Border;
                    imageBorder.Background.Opacity = 0.6;

                    StackPanel stackPanel = grid.Children[2] as StackPanel;
                    stackPanel.Opacity = 0.6;
                }
            }
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

        private void LoadCheatButton_Click(object sender, RoutedEventArgs e) {
            LoadCheatButton.Cursor = Cursors.Wait;

            bool download = true;

            string clientPath = Files.GetGameFolder(Cheats[SelectedCheat].Name) + SDK.DecryptString("client.dll");
            if (File.Exists(clientPath)) {
                char[] buffer = new char[2];
                using (BinaryReader reader = new BinaryReader(new FileStream(clientPath, FileMode.Open))) {
                    reader.Read(buffer, 0, 2);
                    reader.Close();
                }

                int version = -1;
                if (int.TryParse(new string(buffer), out version)) {
                    if (Cheats[SelectedCheat].Version == version) {
                        // ez
                        download = false;
                    }
                }
            }

            // bypasses
            if (Cheats[SelectedCheat].CheatID == Classes.eCheats.CHEAT_RED_DEAD_REDEMPTION_2) {
                string bypassPath = Files.GetGameFolder(Cheats[SelectedCheat].Name) + SDK.DecryptString("bypass.dll");
                if (!File.Exists(bypassPath)) {
                    if (!Network.DownloadBypass(LoginWindow.Username, LoginWindow.Password)) {
                        Utils.ErrorMessage(Network.LastErrorString);
                        LoadCheatButton.Cursor = Cursors.None;
                        return;
                    }
                }

                if (File.Exists(Files.GetGameFolder(Cheats[SelectedCheat].Name) + SDK.DecryptString("Version.txt"))) {
                    int fileVersion = 0;
                    if (int.TryParse(File.ReadAllText(Files.GetGameFolder(Cheats[SelectedCheat].Name) + SDK.DecryptString("Version.txt")), out fileVersion)) {
                        if (fileVersion == Cheats[SelectedCheat].Version) {
                            // ez
                            download = false;
                        }
                    }

                    File.WriteAllText(Files.GetGameFolder(Cheats[SelectedCheat].Name) + SDK.DecryptString("Version.txt"), Cheats[SelectedCheat].Version.ToString());
                }
            }

            // only download if the version is updated on the server
            if (download) {
                MessageBox.Show(Translation.GetString(SDK.DecryptString("If it takes a long time to download, you can use our manual files from the website. You'll need to use our launcher to load the cheat after downloading the client files to the location specified in the rar")));
                if (!Network.DownloadCheat(LoginWindow.Username, LoginWindow.Password, Cheats[SelectedCheat])) {
                    Utils.ErrorMessage(Network.LastErrorString);
                    LoadCheatButton.Cursor = Cursors.None;
                    return;
                }
            }

            // check if process is open
            if (Process.GetProcessesByName(Cheats[SelectedCheat].Process.Substring(0, Cheats[SelectedCheat].Process.Length - 4)).Length > 0) {
                if (Cheats[SelectedCheat].CheatID == Classes.eCheats.CHEAT_RED_DEAD_REDEMPTION_2) {
                    clientPath = Files.GetGameFolder(Cheats[SelectedCheat].Name) + SDK.DecryptString("bypass.dll");
                }

                // inject with no delay, if the process is already running we don't want to delay it
                if (Module.FFFFFFFF(clientPath, Cheats[SelectedCheat].Process)) {
                    Environment.Exit(0);
                } else {
                    Utils.ErrorMessage(Translation.GetString(SDK.DecryptString("Failed loading cheat!")));
                }
            } else {
                // process isn't running, show waiting window
                Hide();

                WaitingWindow waitingWindow = new WaitingWindow();
                waitingWindow.SelectedCheat = Cheats[SelectedCheat];
                waitingWindow.Show();
            }
        }

        private void ViewResourcesButton_Click(object sender, RoutedEventArgs e) {
            ResourcesWindow.SelectedTheme = 0;
            ResourcesWindow.Themes.Clear();

            ResourcesWindow.SelectedTranslation = 0;
            ResourcesWindow.Translations.Clear();

            ResourcesWindow window = new ResourcesWindow();

            // download list of resources
            string resources = Network.GetResources(Cheats[SelectedCheat].Name);
            if (resources != null) {
                JObject jObject = JObject.Parse(resources);
                if (jObject.Count > 0) {
                    JArray jArrayThemes = jObject.Value<JArray>(SDK.DecryptString("themes"));
                    if (jArrayThemes.Count > 0) {
                        int counter = 0;
                        foreach (JObject theme in jArrayThemes) {
                            Classes.Resource t = new Classes.Resource();
                            t.Name = theme.Value<string>(SDK.DecryptString("name"));
                            t.Author = theme.Value<string>(SDK.DecryptString("author"));
                            t.Folder = theme.Value<string>(SDK.DecryptString("folder"));
                            t.Size = theme.Value<int>(SDK.DecryptString("size"));
                            t.Index = counter;

                            t.BorderHandle = CreateListItem(counter++, t.Name, t.Author, new BitmapImage(new Uri(SDK.DecryptString("https://ozark.gg/server/cheats/") + Cheats[SelectedCheat].Name + SDK.DecryptString("/themes/") + t.Folder + SDK.DecryptString("/preview.jpg"))),
                                window.ThemePanelBorder_MouseEnter, window.ThemePanelBorder_MouseLeave, window.ThemePanelBorder_MouseDown);

                            ResourcesWindow.Themes.Add(t);
                            window.ThemePanel.Children.Add(t.BorderHandle);
                        }
                    }

                    JArray jArrayTranslations = jObject.Value<JArray>(SDK.DecryptString("translations"));
                    if (jArrayTranslations.Count > 0) {
                        int counter = 0;
                        foreach (JObject translation in jArrayTranslations) {
                            Classes.Resource t = new Classes.Resource();
                            t.Name = translation.Value<string>(SDK.DecryptString("name"));
                            t.Author = translation.Value<string>(SDK.DecryptString("author"));
                            t.Folder = translation.Value<string>(SDK.DecryptString("folder"));
                            t.Size = translation.Value<int>(SDK.DecryptString("size"));
                            t.Index = counter;

                            t.BorderHandle = CreateListItem(counter++, t.Name, t.Author, new BitmapImage(new Uri(SDK.DecryptString("https://ozark.gg/server/cheats/") + Cheats[SelectedCheat].Name + SDK.DecryptString("/translations/") + t.Folder + SDK.DecryptString("/preview.jpg"))),
                                window.TranslationPanelBorder_MouseEnter, window.TranslationPanelBorder_MouseLeave, window.TranslationPanelBorder_MouseDown);

                            ResourcesWindow.Translations.Add(t);
                            window.TranslationPanel.Children.Add(t.BorderHandle);
                        }
                    }
                }

                window.UpdateLabels();
                window.Show();
            }
        }

        private void ViewChangelogButton_Click(object sender, RoutedEventArgs e) {
            string changelog = Network.GetChangelog(Cheats[SelectedCheat].Name);
            if (changelog != null) {
                string[] lines = changelog.Split(new[] { "\r\n", "\r", "\n" }, StringSplitOptions.None);

                ChangelogWindow window = new ChangelogWindow();

                foreach (var line in lines) {
                    Label label = new Label();
                    label.Content = line;
                    label.Foreground = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#DDDDDD"));
                    label.FontSize = 16;

                    // window.ChangelogPanel.Children.Add(label);

                    TextBlock block = new TextBlock();
                    block.Text = line;
                    block.TextWrapping = TextWrapping.Wrap;
                    block.FontSize = 16;
                    block.Foreground = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#DDDDDD"));

                    window.ChangelogPanel.Children.Add(block);
                }

                // show changelog window
                window.Show();
            }
        }
    }
}
