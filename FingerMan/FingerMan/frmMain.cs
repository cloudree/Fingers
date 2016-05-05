using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using AForge.Video;
using AForge.Video.DirectShow;

namespace FingerMan
{
    public partial class frmMain : Form
    {
        // Serial
        const int SERIAL_SPEED = 115200;

        // Finger Control
        const int MAX_FINGER = 5;
        protected int[] xPos;
        const int MAX_BUFFER_LINE = 1000;
        enum FINGER_ACTION {
            FA_NONE = 0,
            FA_TOUCH = 1,
            FA_DOWN = 2,
            FA_UP = 3,
            FA_LEFT = 4,
            FA_RIGHT = 5,
        };
        const int MOVE_OFFSET = 10;

        // Video
        private FilterInfoCollection videoDevices;
        private VideoCaptureDevice videoDevice;
        private VideoCapabilities[] videoCapabilities;

        //  160x121
        //  ct = center : 80
        //  off2 = +/- 30
        //  off1 = +/- 15
        //  itr = 2

        const float wd = 160.0f, ht = 121.0f;

        // x positions
        const int offx1 = 28, offx2 = offx1 * 2 + 2;
        const int ctx = (int)(wd * 0.5f), itx = 1;
        int[] ptx = new int[MAX_FINGER * 2] {
            ctx - offx2 - itx, ctx - offx2 + itx,
            ctx - offx1 - itx, ctx - offx1 + itx,
            ctx - itx, ctx + itx,
            ctx + offx1 - itx, ctx + offx1 + itx,
            ctx + offx2 - itx, ctx + offx2 + itx
        };
        float[] pointX = new float[MAX_FINGER * 2];

        // y positions
        const int offy1 = 1, offy2 = 1;
        const int cty = (int)(ht * 0.735f);
        int[] pty = new int[MAX_FINGER * 2]
        {
            cty - offy2, cty - offy2,
            cty - offy1, cty - offy1,
            cty, cty,
            cty - offy1, cty - offy1,
            cty - offy2, cty - offy2
        };
        float[] pointY = new float[MAX_FINGER * 2];

        // System
        private bool gameStarted = false;
        const float tickPerSecond = 10000000.0f;

        // ================== Main Form =======================================
        public frmMain()
        {
            InitializeComponent();
            xPos = new int[MAX_FINGER];
            for (int i = 0; i < MAX_FINGER; i++)
            {
                xPos[i] = -1;
            }

            for( int i=0; i<MAX_FINGER*2; i++)
            {
                pointX[i] = ptx[i] / wd;
                pointY[i] = pty[i] / ht;
            }
        }
        
        private void frmMain_Load(object sender, EventArgs e)
        {
            SerialControlsEnable(false);

            // enumerate video devices
            videoDevices = new FilterInfoCollection(FilterCategory.VideoInputDevice);
            if (videoDevices.Count != 0)
            {
                foreach (FilterInfo device in videoDevices)
                {
                    cbDevices.Items.Add(device.Name);
                }
            }
            else
            {
                cbDevices.Items.Add("No DirectShow devices found");
            }
            cbDevices.SelectedIndex = 0;
            CameraControlsEnable(true);
            SetupColorPositions();
        }

        private void ftmMain_Unload(object sender, FormClosingEventArgs e)
        {
            // stop serial port
            serialPort.Close();
            // stop video device
            videoSourcePlayer.SignalToStop();
            videoSourcePlayer.WaitForStop();
            videoSourcePlayer.VideoSource = null;
        }
        
        delegate void SetTextCallback(string text);

        private void LogAddText(string text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.lbText.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(LogAddText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.lbText.Items.Add(text);
                int count = this.lbText.Items.Count;
                while (count > MAX_BUFFER_LINE)
                {
                    this.lbText.Items.RemoveAt(0);
                    count = this.lbText.Items.Count;
                }
                if (count > 0)
                    this.lbText.SelectedIndex = count - 1;
            }
        }

        delegate void SetFPSCallback(string text);

        private void FPSSetText(string text)
        {
            if (this.lbText.InvokeRequired)
            {
                SetFPSCallback d = new SetFPSCallback(FPSSetText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.stFPS.Text = text;
            }
        }

        // ================== Serial Ports ====================================
        private void SerialControlsEnable( bool enable )
        {
            ebSend.Enabled = enable;
            btnSend.Enabled = enable;
            lbText.Enabled = enable;
            cbComPort.Enabled = !enable;
            btnPush1.Enabled = enable;
            btnPush2.Enabled = enable;
            btnPush3.Enabled = enable;
            btnPush4.Enabled = enable;
            btnPush5.Enabled = enable;
        }

        private void cbComPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            String name = cbComPort.Text;
            if (name.Length > 0)
            {
                if (serialPort.IsOpen)
                    serialPort.Close();
                serialPort.PortName = name;
                serialPort.BaudRate = SERIAL_SPEED;
                serialPort.Open();
                this.SerialControlsEnable(true);
                LogAddText("COM Port Opened");
            }
        }
        
        private void ComReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            LogAddText(serialPort.ReadLine() + "\n");
        }

        private void ComErrored(object sender, System.IO.Ports.SerialErrorReceivedEventArgs e)
        {
            MessageBox.Show("Com Port Error", "Error");
        }

        private void SerialSend(String buffer)
        {
            if (serialPort.IsOpen)
            {
                LogAddText(buffer);
                serialPort.WriteLine(buffer);
            }
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            String buffer = ebSend.Text + "\r\n";
            SerialSend(buffer);
        }

        private void SendData(int no, FINGER_ACTION fa)
        {
            String buffer = "";
            for( int i=0; i< MAX_FINGER; i ++)
            {
                if (i == no)
                {
                    switch(fa)
                    {
                        case FINGER_ACTION.FA_TOUCH:
                            buffer += "1";
                            break;
                        case FINGER_ACTION.FA_DOWN:
                            buffer += "2";
                            break;
                        case FINGER_ACTION.FA_UP:
                            buffer += "3";
                            break;
                        case FINGER_ACTION.FA_LEFT:
                            buffer += "4";
                            break;
                        case FINGER_ACTION.FA_RIGHT:
                            buffer += "5";
                            break;
                    }
                }
                else 
                    buffer += "0";
            }
            SerialSend(buffer);
        }

        private void btnTouch(int no, int X)
        {
            SendData(no, FINGER_ACTION.FA_TOUCH);
        }

        private void btnDown(int no, int X)
        {
            xPos[no] = X;
            SendData(no, FINGER_ACTION.FA_DOWN);
        }

        private void btnUp(int no, int X)
        {
            xPos[no] = -1;
            SendData(no, FINGER_ACTION.FA_UP);
        }

        private void btnMove(int no, int X)
        {
            if (xPos[no] >= 0)
            {
                if (X < xPos[no] - MOVE_OFFSET)
                {
                    SendData(no, FINGER_ACTION.FA_LEFT);
                    xPos[no] = -1;
                }
                else if (xPos[no] + MOVE_OFFSET < X)
                {
                    SendData(no, FINGER_ACTION.FA_RIGHT);
                    xPos[no] = -1;
                }
            }
        }

        private void btnPush1_MouseDown(object sender, MouseEventArgs e)
        {
            btnDown(0, e.X);
        }

        private void btnPush1_MouseUp(object sender, MouseEventArgs e)
        {
            btnUp(0, e.X);
        }

        private void btnPush1_MouseMove(object sender, MouseEventArgs e)
        {
            btnMove(0, e.X);
        }

        private void btnPush2_MouseDown(object sender, MouseEventArgs e)
        {
            btnDown(1, e.X);
        }

        private void btnPush2_MouseUp(object sender, MouseEventArgs e)
        {
            btnUp(1, e.X);
        }

        private void btnPush2_MouseMove(object sender, MouseEventArgs e)
        {
            btnMove(1, e.X);
        }

        private void btnPush3_MouseDown(object sender, MouseEventArgs e)
        {
            btnDown(2, e.X);
        }

        private void btnPush3_MouseUp(object sender, MouseEventArgs e)
        {
            btnUp(2, e.X);
        }

        private void btnPush3_MouseMove(object sender, MouseEventArgs e)
        {
            btnMove(2, e.X);
        }

        private void btnPush4_MouseDown(object sender, MouseEventArgs e)
        {
            btnDown(3, e.X);
        }

        private void btnPush4_MouseUp(object sender, MouseEventArgs e)
        {
            btnUp(3, e.X);
        }

        private void btnPush4_MouseMove(object sender, MouseEventArgs e)
        {
            btnMove(3, e.X);
        }

        private void btnPush5_MouseDown(object sender, MouseEventArgs e)
        {
            btnDown(4, e.X);
        }

        private void btnPush5_MouseUp(object sender, MouseEventArgs e)
        {
            btnUp(4, e.X);
        }

        private void btnPush5_MouseMove(object sender, MouseEventArgs e)
        {
            btnMove(4, e.X);
        }

        // ======== Camera ====================================================
        private void CameraControlsEnable(bool enable)
        {
            cbDevices.Enabled = enable;
            cbResolutions.Enabled = enable;
            btnCameraOn.Enabled = enable;
            btnCameraOff.Enabled = !enable;
            btnGameStart.Enabled = !enable;
            btnGameStop.Enabled = !enable;
            btnCapture.Enabled = !enable;
        }

        private void cbDevices_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (videoDevices.Count != 0)
            {
                videoDevice = new VideoCaptureDevice(videoDevices[cbDevices.SelectedIndex].MonikerString);
                EnumeratedSupportedFrameSizes(videoDevice);
            }
        }

        // Collect supported video and snapshot sizes
        private void EnumeratedSupportedFrameSizes(VideoCaptureDevice videoDevice)
        {
            this.Cursor = Cursors.WaitCursor;

            cbResolutions.Items.Clear();
            //snapshotResolutionsCombo.Items.Clear();

            try
            {
                videoCapabilities = videoDevice.VideoCapabilities;
                //snapshotCapabilities = videoDevice.SnapshotCapabilities;

                foreach (VideoCapabilities capabilty in videoCapabilities)
                {
                    // 4:3 only
                    if (capabilty.FrameSize.Width * 3 != capabilty.FrameSize.Height * 4)
                        continue;
                    cbResolutions.Items.Add(string.Format("{0} x {1}",
                        capabilty.FrameSize.Width, capabilty.FrameSize.Height));
                }

                /*
                foreach (VideoCapabilities capabilty in snapshotCapabilities)
                {
                    snapshotResolutionsCombo.Items.Add(string.Format("{0} x {1}",
                        capabilty.FrameSize.Width, capabilty.FrameSize.Height));
                }
                */

                if (videoCapabilities.Length == 0)
                {
                    cbResolutions.Items.Add("Not supported");
                }
                /*
                if (snapshotCapabilities.Length == 0)
                {
                    snapshotResolutionsCombo.Items.Add("Not supported");
                }
                */

                cbResolutions.SelectedIndex = 0;
                //snapshotResolutionsCombo.SelectedIndex = 0;
            }
            finally
            {
                this.Cursor = Cursors.Default;
            }
        }

        private void btnCameraOn_Click(object sender, EventArgs e)
        {
            if (videoDevice != null)
            {
                if ((videoCapabilities != null) && (videoCapabilities.Length != 0))
                {
                    videoDevice.VideoResolution = videoCapabilities[cbResolutions.SelectedIndex];
                }

                /*
                if ((snapshotCapabilities != null) && (snapshotCapabilities.Length != 0))
                {
                    videoDevice.ProvideSnapshots = true;
                    videoDevice.SnapshotResolution = snapshotCapabilities[snapshotResolutionsCombo.SelectedIndex];
                    videoDevice.SnapshotFrame += new NewFrameEventHandler(videoDevice_SnapshotFrame);
                }
                */

                CameraControlsEnable(false);
                GameStartEnable(true);

                videoSourcePlayer.VideoSource = videoDevice;
                videoSourcePlayer.Start();
            }
            gameStarted = false;
        }

        private void btnCameraOff_Click(object sender, EventArgs e)
        {
            if (videoSourcePlayer.VideoSource != null)
            {
                // stop video device
                videoSourcePlayer.SignalToStop();
                videoSourcePlayer.WaitForStop();
                videoSourcePlayer.VideoSource = null;

                /*
                if (videoDevice.ProvideSnapshots)
                {
                    videoDevice.SnapshotFrame -= new NewFrameEventHandler(videoDevice_SnapshotFrame);
                }
                */

                CameraControlsEnable(true);
                GameStartEnable(false);
            }
        }

        private void GameStartEnable(bool en)
        {
            btnGameStart.Enabled = en;
            btnGameStop.Enabled = !en;
        }

        private void btnGameStart_Click(object sender, EventArgs e)
        {
            gameStarted = true;
            GameStartEnable(false);
        }

        private void btnGameStop_Click(object sender, EventArgs e)
        {
            gameStarted = false;
            GameStartEnable(true);
        }

        private void btnCapture_Click(object sender, EventArgs e)
        {
            saveFileDialog1.DefaultExt = "png";
            saveFileDialog1.Filter = "PNG Files (*.png)|*.png|All Files (*.*)|*.*";
            saveFileDialog1.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            DialogResult result = saveFileDialog1.ShowDialog();
            if (result == DialogResult.OK)
            {
                Bitmap bmp = videoSourcePlayer.GetCurrentVideoFrame();
                bmp.Save(saveFileDialog1.FileName, ImageFormat.Png);
                bmp.Dispose();
            }
        }

        private long lastTick = 0, frames = 0;
        private void CalcFPS()
        {
            DateTime currentDate = DateTime.Now;
            long currentTick = currentDate.Ticks;
            frames++;

            if (lastTick == 0)
            {
                lastTick = currentTick;
                return;
            }

            long diffTick = currentTick - lastTick;
            FPSSetText(String.Format("{0:F1} FPS", frames * tickPerSecond / diffTick));

            if (diffTick > tickPerSecond)
            {
                lastTick = currentTick;
                frames = 0;
            }
        }

        // ======== Image Analysis ============================================

        private void SetupPoint( int idx, float fx, float fy )
        {
            Point pt = new Point(
                (int)(videoSourcePlayer.Location.X + videoSourcePlayer.Size.Width * fx),
                (int)(videoSourcePlayer.Location.Y + videoSourcePlayer.Size.Height * fy));

            switch( idx )
            {
                case 0: st0.Location = pt; break;
                case 1: st1.Location = pt; break;
                case 2: st2.Location = pt; break;
                case 3: st3.Location = pt; break;
                case 4: st4.Location = pt; break;
                case 5: st5.Location = pt; break;
                case 6: st6.Location = pt; break;
                case 7: st7.Location = pt; break;
                case 8: st8.Location = pt; break;
                case 9: st9.Location = pt; break;
            }
        }

        private enum LONG_STATE
        {
            LS_NONE = 0,
            LS_LONG
        };
        private LONG_STATE[] state = new LONG_STATE[MAX_FINGER];
        private ColorChangeDetector[] ccd = new ColorChangeDetector[MAX_FINGER * 2];

        private void SetupColorPositions()
        {
            for( int i=0; i<MAX_FINGER*2; i++)
            {
                ccd[i] = new ColorChangeDetector();
                ccd[i].setPoint(pointX[i], pointY[i]);
                SetupPoint(i, pointX[i], pointY[i]);
            }

            for (int i = 0; i<MAX_FINGER; i++)
                state[i] = LONG_STATE.LS_NONE;
        }

        public void ImageChanged(object sender, ref Bitmap image)
        {
            if (!gameStarted)
                return;

            ColorChangeDetector.RoughColor rc, rc2;
            int[] detect = new int[MAX_FINGER] { 0,0,0,0,0 };

            for ( int i=0; i< MAX_FINGER*2; i++)
            {
                Color col = new Color ();
                rc = ccd[i].getChangedColor(ref image, ref col);
                if (rc == ColorChangeDetector.RoughColor.RC_NOTCHANGED) continue;
                if (rc == ColorChangeDetector.RoughColor.RC_UNKNOWN) continue;

                if (false)
                {
                    String Buffer;
                    Buffer = String.Format("@{0} C={1} R={2} G={3} B={4}", i, rc, col.R, col.G, col.B);
                    LogAddText(Buffer);
                }

                int pair = (i % 2 == 0) ? i + 1 : i - 1;
                switch (rc)
                {
                    case ColorChangeDetector.RoughColor.RC_RED:         // touch
                        detect[i / 2] = 1;
                        state[i / 2] = LONG_STATE.LS_NONE;
                        break;
                    case ColorChangeDetector.RoughColor.RC_ORANGE:      // long start
                        if (state[i/2] == LONG_STATE.LS_NONE)
                        {
                            detect[i / 2] = 2;
                            state[i / 2] = LONG_STATE.LS_LONG;
                        }
                        else
                        {
                            detect[i / 2] = 3;
                            state[i / 2] = LONG_STATE.LS_NONE;
                        }
                        break;
                    case ColorChangeDetector.RoughColor.RC_BLUE:        // slide
                        rc2 = ccd[pair].getColor(ref image);
                        if (rc2 == ColorChangeDetector.RoughColor.RC_WHITE)
                        {
                            detect[i / 2] = (i % 2 == 0) ? 4 : 5;
                            state[i / 2] = LONG_STATE.LS_NONE;
                        }
                        break;
                }
            }

            String buffer = "";
            for(int i=0; i< MAX_FINGER; i++)
            {
                buffer += String.Format("{0,1:D}", detect[i]);
            }

            if (buffer.CompareTo("00000") != 0)
            {
                // send to Serial immediately
                SerialSend(buffer);
            }

            CalcFPS();
        }

        // end of class frmMain
    }
}
