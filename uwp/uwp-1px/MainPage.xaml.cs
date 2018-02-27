using System;
using Windows.ApplicationModel.Core;
using Windows.System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Core;
using Windows.UI.Xaml.Media;
using Windows.UI;

namespace uwp_1px
{
    public sealed partial class MainPage : Page
    {
        DispatcherTimer timer;
        int idx = 0;

        public MainPage()
        {
            this.InitializeComponent();

            // timer
            timer = new DispatcherTimer();
            timer.Interval = new TimeSpan(0, 0, 0, 0, 250); // 250ms
            timer.Tick += this.HandleTimer;
            timer.Start();

            // exit when pressing escape key
            Window.Current.CoreWindow.KeyDown += HandleKeyEvent;
        }

        private void HandleTimer(object sender, object e)
        {
            idx++;
            if (idx == 4) idx = 0;

            switch (idx) {
                case 0:
                    this.canvas.Background = new SolidColorBrush(Color.FromArgb(255, 255, 0, 0));
                    break;
                case 1:
                    this.canvas.Background = new SolidColorBrush(Color.FromArgb(255, 0, 255, 0));
                    break;
                case 2:
                    this.canvas.Background = new SolidColorBrush(Color.FromArgb(255, 0, 0, 255));
                    break;
                case 3:
                    this.canvas.Background = new SolidColorBrush(Color.FromArgb(255, 0, 0, 0));
                    break;
            }
        }

        private void HandleKeyEvent(CoreWindow sender, KeyEventArgs args)
        {
            if (args.VirtualKey == VirtualKey.Escape)
            {
                CoreApplication.Exit();
            }
        }
    }
}
