using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
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
    /// Interaction logic for ResourcesWindow.xaml
    /// </summary>
    public partial class ResourcesWindow : Window {
        public static List<Classes.Resource> Themes = new List<Classes.Resource>();
        public static int SelectedTheme = 0;

        public static List<Classes.Resource> Translations = new List<Classes.Resource>();
        public static int SelectedTranslation = 0;

        public static List<Classes.Resource> BulletModifiers = new List<Classes.Resource>();
        public static int SelectedBulletModifier = 0;

        public ResourcesWindow() {
            InitializeComponent();

            ThemesHeader.Header = Translation.GetString(ThemesHeader.Header as string);
            TranslationsHeader.Header = Translation.GetString(TranslationsHeader.Header as string);
        }

        void ResetLabels() {
            PropertiesLabel1.Content = "";
            PropertiesLabel1Result.Content = "";

            PropertiesLabel2.Content = "";
            PropertiesLabel2Result.Content = "";

            PropertiesLabel3.Content = "";
            PropertiesLabel3Result.Content = "";

            PropertiesLabel4.Content = "";
            PropertiesLabel4Result.Content = "";

            PropertiesLabel5.Content = "";
            PropertiesLabel5Result.Content = "";

            SelectedPreviewImage.Source = null;
            Button.Content = "";
        }

        public void UpdateLabels() {
            ResetLabels();

            // Theme
            if (Tabs.SelectedIndex == 0) {
                if (SelectedTheme == null) return;

                PropertiesLabel1.Content = Translation.GetString(SDK.DecryptString("Name:"));
                PropertiesLabel2.Content = Translation.GetString(SDK.DecryptString("Author:"));
                PropertiesLabel3.Content = Translation.GetString(SDK.DecryptString("Size:"));
                Button.Content = Translation.GetString(SDK.DecryptString("Download Theme"));

                if (Themes.Count > 0) {
                    PropertiesLabel1Result.Content = Themes[SelectedTheme].Name;
                    PropertiesLabel1Result.Margin = Utils.CalculateLabelPosition(PropertiesLabel1, PropertiesLabel1Result);

                    PropertiesLabel2Result.Content = Themes[SelectedTheme].Author;
                    PropertiesLabel2Result.Margin = Utils.CalculateLabelPosition(PropertiesLabel2, PropertiesLabel2Result);

                    PropertiesLabel3Result.Content = Utils.BytesToString(Themes[SelectedTheme].Size);
                    PropertiesLabel3Result.Margin = Utils.CalculateLabelPosition(PropertiesLabel3, PropertiesLabel3Result);

                    Grid grid = Themes[SelectedTheme].BorderHandle.Child as Grid;
                    Border imageBorder = grid.Children[1] as Border;
                    ImageBrush imageBrush = imageBorder.Background as ImageBrush;
                    ImageSource imageSource = imageBrush.ImageSource;

                    SelectedPreviewImage.Source = imageSource;
                } else {
                    SelectedPreviewImage.Source = new BitmapImage();
                }
            }

            // Translation
            if (Tabs.SelectedIndex == 1) {
                PropertiesLabel1.Content = Translation.GetString(SDK.DecryptString("Name:"));
                PropertiesLabel2.Content = Translation.GetString(SDK.DecryptString("Author:"));
                PropertiesLabel3.Content = Translation.GetString(SDK.DecryptString("Size:"));
                Button.Content = Translation.GetString(SDK.DecryptString("Download Translation"));

                if (Translations.Count > 0) {
                    PropertiesLabel1Result.Content = Translations[SelectedTranslation].Name;
                    PropertiesLabel1Result.Margin = Utils.CalculateLabelPosition(PropertiesLabel1, PropertiesLabel1Result);

                    PropertiesLabel2Result.Content = Translations[SelectedTranslation].Author;
                    PropertiesLabel2Result.Margin = Utils.CalculateLabelPosition(PropertiesLabel2, PropertiesLabel2Result);

                    PropertiesLabel3Result.Content = Utils.BytesToString(Translations[SelectedTranslation].Size);
                    PropertiesLabel3Result.Margin = Utils.CalculateLabelPosition(PropertiesLabel3, PropertiesLabel3Result);

                    Grid grid = Translations[SelectedTranslation].BorderHandle.Child as Grid;
                    Border imageBorder = grid.Children[1] as Border;
                    ImageBrush imageBrush = imageBorder.Background as ImageBrush;
                    ImageSource imageSource = imageBrush.ImageSource;

                    SelectedPreviewImage.Source = imageSource;
                } else {
                    SelectedPreviewImage.Source = new BitmapImage();
                }
            }
        }

        Classes.Resource GetThemeFromBorder(Border handle) {
            foreach (var theme in Themes) {
                if (theme.BorderHandle == handle) {
                    return theme;
                }
            }

            return null;
        }

        Classes.Resource GetTranslationFromBorder(Border handle) {
            foreach (var theme in Translations) {
                if (theme.BorderHandle == handle) {
                    return theme;
                }
            }

            return null;
        }

        public void TranslationPanelBorder_MouseDown(object sender, MouseButtonEventArgs e) {
            Classes.Resource translation = GetTranslationFromBorder(sender as Border);
            if (translation != null) {
                SelectedTranslation = translation.Index;

                UpdateLabels();

                // update each event by emulating a mouseleave event on each border
                foreach (var child in TranslationPanel.Children) {
                    TranslationPanelBorder_MouseLeave(child as Border, null);
                }
            }
        }

        public void TranslationPanelBorder_MouseEnter(object sender, MouseEventArgs e) {
            Classes.Resource translation = GetTranslationFromBorder(sender as Border);
            if (translation != null) {
                translation.BorderHandle.Cursor = Cursors.Hand;

                Grid grid = translation.BorderHandle.Child as Grid;

                Border rectangleBorder = grid.Children[0] as Border;
                rectangleBorder.Background = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#424447"));

                Border imageBorder = grid.Children[1] as Border;
                imageBorder.Background.Opacity = 1.0;

                StackPanel stackPanel = grid.Children[2] as StackPanel;
                stackPanel.Opacity = 1.0;
            }
        }

        public void TranslationPanelBorder_MouseLeave(object sender, MouseEventArgs e) {
            Classes.Resource translation = GetTranslationFromBorder(sender as Border);
            if (translation != null) {
                if (e != null) translation.BorderHandle.Cursor = Cursors.None;

                if (translation.Index != SelectedTranslation) {
                    Grid grid = translation.BorderHandle.Child as Grid;

                    Border rectangleBorder = grid.Children[0] as Border;
                    rectangleBorder.Background = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#252729"));

                    Border imageBorder = grid.Children[1] as Border;
                    imageBorder.Background.Opacity = 0.6;

                    StackPanel stackPanel = grid.Children[2] as StackPanel;
                    stackPanel.Opacity = 0.6;
                }
            }
        }
        
        public void ThemePanelBorder_MouseDown(object sender, MouseButtonEventArgs e) {
            Classes.Resource theme = GetThemeFromBorder(sender as Border);
            if (theme != null) {
                SelectedTheme = theme.Index;

                UpdateLabels();

                // update each event by emulating a mouseleave event on each border
                foreach (var child in ThemePanel.Children) {
                    ThemePanelBorder_MouseLeave(child as Border, null);
                }
            }
        }

        public void ThemePanelBorder_MouseEnter(object sender, MouseEventArgs e) {
            Classes.Resource theme = GetThemeFromBorder(sender as Border);
            if (theme != null) {
                theme.BorderHandle.Cursor = Cursors.Hand;

                Grid grid = theme.BorderHandle.Child as Grid;

                Border rectangleBorder = grid.Children[0] as Border;
                rectangleBorder.Background = (Brush)new BrushConverter().ConvertFromString(SDK.DecryptString("#424447"));

                Border imageBorder = grid.Children[1] as Border;
                imageBorder.Background.Opacity = 1.0;

                StackPanel stackPanel = grid.Children[2] as StackPanel;
                stackPanel.Opacity = 1.0;
            }
        }

        public void ThemePanelBorder_MouseLeave(object sender, MouseEventArgs e) {
            Classes.Resource theme = GetThemeFromBorder(sender as Border);
            if (theme != null) {
                if (e != null) theme.BorderHandle.Cursor = Cursors.None;

                if (theme.Index != SelectedTheme) {
                    Grid grid = theme.BorderHandle.Child as Grid;

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
                Close();
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
                System.Diagnostics.Process.Start(SDK.DecryptString("https://ozark.gg"));
            }
        }

        private void Tabs_SelectionChanged(object sender, SelectionChangedEventArgs e) {
            UpdateLabels();
        }

        private void Button_Click(object sender, RoutedEventArgs e) {
            // Themes
            if (Tabs.SelectedIndex == 0) {
                string resource = Network.GetResource(MainWindow.Cheats[MainWindow.SelectedCheat].Name, SDK.DecryptString("themes"), Themes[SelectedTheme].Folder, SDK.DecryptString("file.json"));
                if (resource != null) {
                    File.WriteAllText(Files.GetGameFolder(MainWindow.Cheats[MainWindow.SelectedCheat].Name) + SDK.DecryptString("Themes\\") + Themes[SelectedTheme].Name + SDK.DecryptString(".json"), resource);
                    Utils.SuccessMessage(Translation.GetString(SDK.DecryptString("Successfully downloaded resource!")));
                    return;
                }
            }

            // Translations
            if (Tabs.SelectedIndex == 1) {
                string resource = Network.GetResource(MainWindow.Cheats[MainWindow.SelectedCheat].Name, SDK.DecryptString("translations"), Translations[SelectedTranslation].Folder, SDK.DecryptString("file.json"));
                if (resource != null) {
                    File.WriteAllText(Files.GetGameFolder(MainWindow.Cheats[MainWindow.SelectedCheat].Name) + SDK.DecryptString("Translations\\") + Translations[SelectedTranslation].Name + SDK.DecryptString(".json"), resource);
                    Utils.SuccessMessage(Translation.GetString(SDK.DecryptString("Successfully downloaded resource!")));
                    return;
                }
            }

            Utils.ErrorMessage(SDK.DecryptString("Failed to download resource!"));
        }
    }
}
