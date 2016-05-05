namespace FingerMan
{
    partial class frmMain
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.cbComPort = new System.Windows.Forms.ComboBox();
            this.ebSend = new System.Windows.Forms.TextBox();
            this.btnSend = new System.Windows.Forms.Button();
            this.btnPush1 = new System.Windows.Forms.Button();
            this.btnPush2 = new System.Windows.Forms.Button();
            this.btnPush3 = new System.Windows.Forms.Button();
            this.btnPush4 = new System.Windows.Forms.Button();
            this.btnPush5 = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.miFile = new System.Windows.Forms.ToolStripMenuItem();
            this.miFileExit = new System.Windows.Forms.ToolStripMenuItem();
            this.miEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.miEditOptions = new System.Windows.Forms.ToolStripMenuItem();
            this.lbText = new System.Windows.Forms.ListBox();
            this.videoSourcePlayer = new AForge.Controls.VideoSourcePlayer();
            this.cbDevices = new System.Windows.Forms.ComboBox();
            this.cbResolutions = new System.Windows.Forms.ComboBox();
            this.btnCameraOn = new System.Windows.Forms.Button();
            this.btnCameraOff = new System.Windows.Forms.Button();
            this.btnCapture = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.stFPS = new System.Windows.Forms.Label();
            this.st0 = new System.Windows.Forms.Label();
            this.st1 = new System.Windows.Forms.Label();
            this.st3 = new System.Windows.Forms.Label();
            this.st2 = new System.Windows.Forms.Label();
            this.st5 = new System.Windows.Forms.Label();
            this.st4 = new System.Windows.Forms.Label();
            this.st7 = new System.Windows.Forms.Label();
            this.st6 = new System.Windows.Forms.Label();
            this.st9 = new System.Windows.Forms.Label();
            this.st8 = new System.Windows.Forms.Label();
            this.btnGameStart = new System.Windows.Forms.Button();
            this.btnGameStop = new System.Windows.Forms.Button();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // serialPort
            // 
            this.serialPort.ErrorReceived += new System.IO.Ports.SerialErrorReceivedEventHandler(this.ComErrored);
            this.serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.ComReceived);
            // 
            // cbComPort
            // 
            this.cbComPort.FormattingEnabled = true;
            this.cbComPort.Items.AddRange(new object[] {
            "COM1",
            "COM2",
            "COM3",
            "COM4",
            "COM5",
            "COM6",
            "COM7",
            "COM8",
            "COM9",
            "COM10",
            "COM11",
            "COM12"});
            this.cbComPort.Location = new System.Drawing.Point(12, 27);
            this.cbComPort.Name = "cbComPort";
            this.cbComPort.Size = new System.Drawing.Size(83, 20);
            this.cbComPort.TabIndex = 0;
            this.cbComPort.SelectedIndexChanged += new System.EventHandler(this.cbComPort_SelectedIndexChanged);
            // 
            // ebSend
            // 
            this.ebSend.Location = new System.Drawing.Point(12, 53);
            this.ebSend.Name = "ebSend";
            this.ebSend.Size = new System.Drawing.Size(229, 21);
            this.ebSend.TabIndex = 2;
            // 
            // btnSend
            // 
            this.btnSend.Location = new System.Drawing.Point(247, 53);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(44, 23);
            this.btnSend.TabIndex = 3;
            this.btnSend.Text = "Send";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // btnPush1
            // 
            this.btnPush1.Location = new System.Drawing.Point(301, 388);
            this.btnPush1.Name = "btnPush1";
            this.btnPush1.Size = new System.Drawing.Size(75, 23);
            this.btnPush1.TabIndex = 5;
            this.btnPush1.Text = "1";
            this.btnPush1.UseVisualStyleBackColor = true;
            this.btnPush1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnPush1_MouseDown);
            this.btnPush1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.btnPush1_MouseMove);
            this.btnPush1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnPush1_MouseUp);
            // 
            // btnPush2
            // 
            this.btnPush2.Location = new System.Drawing.Point(382, 388);
            this.btnPush2.Name = "btnPush2";
            this.btnPush2.Size = new System.Drawing.Size(75, 23);
            this.btnPush2.TabIndex = 6;
            this.btnPush2.Text = "2";
            this.btnPush2.UseVisualStyleBackColor = true;
            this.btnPush2.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnPush2_MouseDown);
            this.btnPush2.MouseMove += new System.Windows.Forms.MouseEventHandler(this.btnPush2_MouseMove);
            this.btnPush2.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnPush2_MouseUp);
            // 
            // btnPush3
            // 
            this.btnPush3.Location = new System.Drawing.Point(463, 388);
            this.btnPush3.Name = "btnPush3";
            this.btnPush3.Size = new System.Drawing.Size(75, 23);
            this.btnPush3.TabIndex = 7;
            this.btnPush3.Text = "3";
            this.btnPush3.UseVisualStyleBackColor = true;
            this.btnPush3.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnPush3_MouseDown);
            this.btnPush3.MouseMove += new System.Windows.Forms.MouseEventHandler(this.btnPush3_MouseMove);
            this.btnPush3.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnPush3_MouseUp);
            // 
            // btnPush4
            // 
            this.btnPush4.Location = new System.Drawing.Point(544, 388);
            this.btnPush4.Name = "btnPush4";
            this.btnPush4.Size = new System.Drawing.Size(75, 23);
            this.btnPush4.TabIndex = 8;
            this.btnPush4.Text = "4";
            this.btnPush4.UseVisualStyleBackColor = true;
            this.btnPush4.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnPush4_MouseDown);
            this.btnPush4.MouseMove += new System.Windows.Forms.MouseEventHandler(this.btnPush4_MouseMove);
            this.btnPush4.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnPush4_MouseUp);
            // 
            // btnPush5
            // 
            this.btnPush5.Location = new System.Drawing.Point(625, 388);
            this.btnPush5.Name = "btnPush5";
            this.btnPush5.Size = new System.Drawing.Size(75, 23);
            this.btnPush5.TabIndex = 9;
            this.btnPush5.Text = "5";
            this.btnPush5.UseVisualStyleBackColor = true;
            this.btnPush5.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnPush5_MouseDown);
            this.btnPush5.MouseMove += new System.Windows.Forms.MouseEventHandler(this.btnPush5_MouseMove);
            this.btnPush5.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnPush5_MouseUp);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miFile,
            this.miEdit});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(714, 24);
            this.menuStrip1.TabIndex = 10;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // miFile
            // 
            this.miFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miFileExit});
            this.miFile.Name = "miFile";
            this.miFile.Size = new System.Drawing.Size(37, 20);
            this.miFile.Text = "File";
            // 
            // miFileExit
            // 
            this.miFileExit.Name = "miFileExit";
            this.miFileExit.Size = new System.Drawing.Size(93, 22);
            this.miFileExit.Text = "Exit";
            // 
            // miEdit
            // 
            this.miEdit.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miEditOptions});
            this.miEdit.Name = "miEdit";
            this.miEdit.Size = new System.Drawing.Size(39, 20);
            this.miEdit.Text = "Edit";
            // 
            // miEditOptions
            // 
            this.miEditOptions.Enabled = false;
            this.miEditOptions.Name = "miEditOptions";
            this.miEditOptions.Size = new System.Drawing.Size(116, 22);
            this.miEditOptions.Text = "Options";
            // 
            // lbText
            // 
            this.lbText.FormattingEnabled = true;
            this.lbText.ItemHeight = 12;
            this.lbText.Location = new System.Drawing.Point(12, 82);
            this.lbText.Name = "lbText";
            this.lbText.Size = new System.Drawing.Size(279, 328);
            this.lbText.TabIndex = 11;
            // 
            // videoSourcePlayer
            // 
            this.videoSourcePlayer.BackColor = System.Drawing.SystemColors.ControlDark;
            this.videoSourcePlayer.ForeColor = System.Drawing.Color.DarkRed;
            this.videoSourcePlayer.Location = new System.Drawing.Point(301, 82);
            this.videoSourcePlayer.Name = "videoSourcePlayer";
            this.videoSourcePlayer.Size = new System.Drawing.Size(400, 300);
            this.videoSourcePlayer.TabIndex = 0;
            this.videoSourcePlayer.VideoSource = null;
            this.videoSourcePlayer.NewFrame += new AForge.Controls.VideoSourcePlayer.NewFrameHandler(this.ImageChanged);
            // 
            // cbDevices
            // 
            this.cbDevices.FormattingEnabled = true;
            this.cbDevices.Location = new System.Drawing.Point(301, 27);
            this.cbDevices.Name = "cbDevices";
            this.cbDevices.Size = new System.Drawing.Size(224, 20);
            this.cbDevices.TabIndex = 12;
            this.cbDevices.SelectedIndexChanged += new System.EventHandler(this.cbDevices_SelectedIndexChanged);
            // 
            // cbResolutions
            // 
            this.cbResolutions.FormattingEnabled = true;
            this.cbResolutions.Location = new System.Drawing.Point(531, 27);
            this.cbResolutions.Name = "cbResolutions";
            this.cbResolutions.Size = new System.Drawing.Size(113, 20);
            this.cbResolutions.TabIndex = 13;
            // 
            // btnCameraOn
            // 
            this.btnCameraOn.Location = new System.Drawing.Point(301, 53);
            this.btnCameraOn.Name = "btnCameraOn";
            this.btnCameraOn.Size = new System.Drawing.Size(75, 23);
            this.btnCameraOn.TabIndex = 14;
            this.btnCameraOn.Text = "CameraOn";
            this.btnCameraOn.UseVisualStyleBackColor = true;
            this.btnCameraOn.Click += new System.EventHandler(this.btnCameraOn_Click);
            // 
            // btnCameraOff
            // 
            this.btnCameraOff.Location = new System.Drawing.Point(382, 53);
            this.btnCameraOff.Name = "btnCameraOff";
            this.btnCameraOff.Size = new System.Drawing.Size(75, 23);
            this.btnCameraOff.TabIndex = 15;
            this.btnCameraOff.Text = "CameraOff";
            this.btnCameraOff.UseVisualStyleBackColor = true;
            this.btnCameraOff.Click += new System.EventHandler(this.btnCameraOff_Click);
            // 
            // btnCapture
            // 
            this.btnCapture.Location = new System.Drawing.Point(625, 53);
            this.btnCapture.Name = "btnCapture";
            this.btnCapture.Size = new System.Drawing.Size(75, 23);
            this.btnCapture.TabIndex = 16;
            this.btnCapture.Text = "Capture...";
            this.btnCapture.UseVisualStyleBackColor = true;
            this.btnCapture.Click += new System.EventHandler(this.btnCapture_Click);
            // 
            // stFPS
            // 
            this.stFPS.AutoSize = true;
            this.stFPS.BackColor = System.Drawing.Color.Transparent;
            this.stFPS.Location = new System.Drawing.Point(311, 91);
            this.stFPS.Name = "stFPS";
            this.stFPS.Size = new System.Drawing.Size(48, 12);
            this.stFPS.TabIndex = 17;
            this.stFPS.Text = "0.0 FPS";
            // 
            // st0
            // 
            this.st0.AutoSize = true;
            this.st0.Location = new System.Drawing.Point(323, 340);
            this.st0.Name = "st0";
            this.st0.Size = new System.Drawing.Size(15, 12);
            this.st0.TabIndex = 18;
            this.st0.Text = "`0";
            // 
            // st1
            // 
            this.st1.AutoSize = true;
            this.st1.Location = new System.Drawing.Point(344, 340);
            this.st1.Name = "st1";
            this.st1.Size = new System.Drawing.Size(15, 12);
            this.st1.TabIndex = 19;
            this.st1.Text = "`1";
            // 
            // st3
            // 
            this.st3.AutoSize = true;
            this.st3.Location = new System.Drawing.Point(424, 340);
            this.st3.Name = "st3";
            this.st3.Size = new System.Drawing.Size(15, 12);
            this.st3.TabIndex = 21;
            this.st3.Text = "`3";
            // 
            // st2
            // 
            this.st2.AutoSize = true;
            this.st2.Location = new System.Drawing.Point(403, 340);
            this.st2.Name = "st2";
            this.st2.Size = new System.Drawing.Size(15, 12);
            this.st2.TabIndex = 20;
            this.st2.Text = "`2";
            // 
            // st5
            // 
            this.st5.AutoSize = true;
            this.st5.Location = new System.Drawing.Point(501, 340);
            this.st5.Name = "st5";
            this.st5.Size = new System.Drawing.Size(15, 12);
            this.st5.TabIndex = 23;
            this.st5.Text = "`5";
            // 
            // st4
            // 
            this.st4.AutoSize = true;
            this.st4.Location = new System.Drawing.Point(480, 340);
            this.st4.Name = "st4";
            this.st4.Size = new System.Drawing.Size(15, 12);
            this.st4.TabIndex = 22;
            this.st4.Text = "`4";
            // 
            // st7
            // 
            this.st7.AutoSize = true;
            this.st7.Location = new System.Drawing.Point(585, 340);
            this.st7.Name = "st7";
            this.st7.Size = new System.Drawing.Size(15, 12);
            this.st7.TabIndex = 25;
            this.st7.Text = "`7";
            // 
            // st6
            // 
            this.st6.AutoSize = true;
            this.st6.Location = new System.Drawing.Point(564, 340);
            this.st6.Name = "st6";
            this.st6.Size = new System.Drawing.Size(15, 12);
            this.st6.TabIndex = 24;
            this.st6.Text = "`6";
            // 
            // st9
            // 
            this.st9.AutoSize = true;
            this.st9.Location = new System.Drawing.Point(664, 340);
            this.st9.Name = "st9";
            this.st9.Size = new System.Drawing.Size(15, 12);
            this.st9.TabIndex = 27;
            this.st9.Text = "`9";
            // 
            // st8
            // 
            this.st8.AutoSize = true;
            this.st8.Location = new System.Drawing.Point(643, 340);
            this.st8.Name = "st8";
            this.st8.Size = new System.Drawing.Size(15, 12);
            this.st8.TabIndex = 26;
            this.st8.Text = "`8";
            // 
            // btnGameStart
            // 
            this.btnGameStart.Location = new System.Drawing.Point(463, 53);
            this.btnGameStart.Name = "btnGameStart";
            this.btnGameStart.Size = new System.Drawing.Size(75, 23);
            this.btnGameStart.TabIndex = 28;
            this.btnGameStart.Text = "GameStart";
            this.btnGameStart.UseVisualStyleBackColor = true;
            this.btnGameStart.Click += new System.EventHandler(this.btnGameStart_Click);
            // 
            // btnGameStop
            // 
            this.btnGameStop.Location = new System.Drawing.Point(544, 53);
            this.btnGameStop.Name = "btnGameStop";
            this.btnGameStop.Size = new System.Drawing.Size(75, 23);
            this.btnGameStop.TabIndex = 29;
            this.btnGameStop.Text = "GameStop";
            this.btnGameStop.UseVisualStyleBackColor = true;
            this.btnGameStop.Click += new System.EventHandler(this.btnGameStop_Click);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(714, 425);
            this.Controls.Add(this.btnGameStop);
            this.Controls.Add(this.btnGameStart);
            this.Controls.Add(this.st9);
            this.Controls.Add(this.st8);
            this.Controls.Add(this.st7);
            this.Controls.Add(this.st6);
            this.Controls.Add(this.st5);
            this.Controls.Add(this.st4);
            this.Controls.Add(this.st3);
            this.Controls.Add(this.st2);
            this.Controls.Add(this.st1);
            this.Controls.Add(this.st0);
            this.Controls.Add(this.stFPS);
            this.Controls.Add(this.btnCapture);
            this.Controls.Add(this.btnCameraOff);
            this.Controls.Add(this.btnCameraOn);
            this.Controls.Add(this.cbResolutions);
            this.Controls.Add(this.cbDevices);
            this.Controls.Add(this.lbText);
            this.Controls.Add(this.btnPush5);
            this.Controls.Add(this.btnPush4);
            this.Controls.Add(this.btnPush3);
            this.Controls.Add(this.btnPush2);
            this.Controls.Add(this.btnPush1);
            this.Controls.Add(this.btnSend);
            this.Controls.Add(this.ebSend);
            this.Controls.Add(this.cbComPort);
            this.Controls.Add(this.videoSourcePlayer);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "frmMain";
            this.Text = "FingerMan";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ftmMain_Unload);
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        public System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.ComboBox cbComPort;
        private System.Windows.Forms.TextBox ebSend;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.Button btnPush1;
        private System.Windows.Forms.Button btnPush2;
        private System.Windows.Forms.Button btnPush3;
        private System.Windows.Forms.Button btnPush4;
        private System.Windows.Forms.Button btnPush5;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem miFile;
        private System.Windows.Forms.ToolStripMenuItem miFileExit;
        private System.Windows.Forms.ToolStripMenuItem miEdit;
        private System.Windows.Forms.ToolStripMenuItem miEditOptions;
        private System.Windows.Forms.ListBox lbText;
        private AForge.Controls.VideoSourcePlayer videoSourcePlayer;
        private System.Windows.Forms.ComboBox cbDevices;
        private System.Windows.Forms.ComboBox cbResolutions;
        private System.Windows.Forms.Button btnCameraOn;
        private System.Windows.Forms.Button btnCameraOff;
        private System.Windows.Forms.Button btnCapture;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Label stFPS;
        private System.Windows.Forms.Label st0;
        private System.Windows.Forms.Label st1;
        private System.Windows.Forms.Label st3;
        private System.Windows.Forms.Label st2;
        private System.Windows.Forms.Label st5;
        private System.Windows.Forms.Label st4;
        private System.Windows.Forms.Label st7;
        private System.Windows.Forms.Label st6;
        private System.Windows.Forms.Label st9;
        private System.Windows.Forms.Label st8;
        private System.Windows.Forms.Button btnGameStart;
        private System.Windows.Forms.Button btnGameStop;
    }
}

