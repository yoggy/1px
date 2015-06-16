using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;
using System.Diagnostics;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace wpf
{
    public partial class MainWindow : Window
    {
        DispatcherTimer dispatcherTimer;
        int idx;
        WriteableBitmap[] images;

        public MainWindow()
        {
            InitializeComponent();

            // prepare images
            images = new WriteableBitmap[4];

            Int32Rect r = new Int32Rect(100, 100, 1, 1);

            byte [] color_r = {0, 0, 255};
            images[0] = new WriteableBitmap(200, 200, 75, 75, PixelFormats.Bgr24, null);
            images[0].WritePixels(r, color_r, 3, 0);

            byte[] color_g = { 0, 255, 0 };
            images[1] = new WriteableBitmap(200, 200, 75, 75, PixelFormats.Bgr24, null);
            images[1].WritePixels(r, color_g, 3, 0);

            byte[] color_b = { 255, 0, 0 };
            images[2] = new WriteableBitmap(200, 200, 75, 75, PixelFormats.Bgr24, null);
            images[2].WritePixels(r, color_b, 3, 0);

            images[3] = new WriteableBitmap(200, 200, 75, 75, PixelFormats.Bgr24, null);

            // timer
            dispatcherTimer = new DispatcherTimer(DispatcherPriority.Normal);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 0, 0, 250); // 250ms
            dispatcherTimer.Tick += new EventHandler(HandleTimer);
            dispatcherTimer.Start();

            // append key event handler
            this.PreviewKeyDown += new KeyEventHandler(HandleKeyEvent);
        }

        void HandleTimer(object sender, EventArgs e)
        {
            idx++;
            if (idx == 4) idx = 0;

            this.image.Source = images[idx];
        }

        private void HandleKeyEvent(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Escape)
            {
                Close();
            }
        }
    }
}
