using System;
using System.Collections.Generic;
using System.Linq;
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
    /// Interaction logic for ChangelogWindow.xaml
    /// </summary>
    public partial class ChangelogWindow : Window {
        public ChangelogWindow() {
            InitializeComponent();
        }

        private void HeaderImage_MouseEnter(object sender, MouseEventArgs e) {
            Image image = sender as Image;
            image.Cursor = Cursors.Hand;
        }

        private void HeaderImage_MouseLeave(object sender, MouseEventArgs e) {
            Image image = sender as Image;
            image.Cursor = Cursors.None;
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

        private void BannerRectangle_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left) {
                DragMove();
            }
        }

        private void URLLabel_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left) {
                System.Diagnostics.Process.Start(SDK.DecryptString("https://ozark.gg"));
            }
        }
    }
}
