namespace OpenCLFilter
{
    partial class frmMainFilter
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMainFilter));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.btnOpenFile = new System.Windows.Forms.ToolStripButton();
            this.btnSave = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnApplyFilter = new System.Windows.Forms.ToolStripButton();
            this.cmbWorkDim = new System.Windows.Forms.ToolStripComboBox();
            this.btnFilterNoOpenCL = new System.Windows.Forms.ToolStripButton();
            this.btnConfigFilter = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.btnStartWebCam = new System.Windows.Forms.ToolStripButton();
            this.btnDeactivateCam = new System.Windows.Forms.ToolStripButton();
            this.lblFps = new System.Windows.Forms.ToolStripLabel();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.btnCLConfig = new System.Windows.Forms.ToolStripButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.pic = new System.Windows.Forms.PictureBox();
            this.toolStrip1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pic)).BeginInit();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnOpenFile,
            this.btnSave,
            this.toolStripSeparator1,
            this.btnApplyFilter,
            this.cmbWorkDim,
            this.btnFilterNoOpenCL,
            this.btnConfigFilter,
            this.toolStripSeparator2,
            this.btnStartWebCam,
            this.btnDeactivateCam,
            this.lblFps,
            this.toolStripSeparator3,
            this.btnCLConfig});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(702, 55);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // btnOpenFile
            // 
            this.btnOpenFile.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnOpenFile.Image = ((System.Drawing.Image)(resources.GetObject("btnOpenFile.Image")));
            this.btnOpenFile.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnOpenFile.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnOpenFile.Name = "btnOpenFile";
            this.btnOpenFile.Size = new System.Drawing.Size(52, 52);
            this.btnOpenFile.Text = "Load picture to be filtered";
            this.btnOpenFile.Click += new System.EventHandler(this.btnOpenFile_Click);
            // 
            // btnSave
            // 
            this.btnSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnSave.Image = ((System.Drawing.Image)(resources.GetObject("btnSave.Image")));
            this.btnSave.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(52, 52);
            this.btnSave.ToolTipText = "Save image";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 55);
            // 
            // btnApplyFilter
            // 
            this.btnApplyFilter.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnApplyFilter.Image = ((System.Drawing.Image)(resources.GetObject("btnApplyFilter.Image")));
            this.btnApplyFilter.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnApplyFilter.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnApplyFilter.Name = "btnApplyFilter";
            this.btnApplyFilter.Size = new System.Drawing.Size(52, 52);
            this.btnApplyFilter.ToolTipText = "Apply filter";
            this.btnApplyFilter.Click += new System.EventHandler(this.btnApplyFilter_Click);
            // 
            // cmbWorkDim
            // 
            this.cmbWorkDim.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbWorkDim.Items.AddRange(new object[] {
            "Work_dim = 1",
            "Work_dim = 2"});
            this.cmbWorkDim.Name = "cmbWorkDim";
            this.cmbWorkDim.Size = new System.Drawing.Size(121, 55);
            this.cmbWorkDim.SelectedIndexChanged += new System.EventHandler(this.cmbWorkDim_SelectedIndexChanged);
            this.cmbWorkDim.Click += new System.EventHandler(this.cmbWorkDim_Click);
            // 
            // btnFilterNoOpenCL
            // 
            this.btnFilterNoOpenCL.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnFilterNoOpenCL.Image = ((System.Drawing.Image)(resources.GetObject("btnFilterNoOpenCL.Image")));
            this.btnFilterNoOpenCL.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnFilterNoOpenCL.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnFilterNoOpenCL.Name = "btnFilterNoOpenCL";
            this.btnFilterNoOpenCL.Size = new System.Drawing.Size(52, 52);
            this.btnFilterNoOpenCL.ToolTipText = "Filter image without using OpenCL";
            this.btnFilterNoOpenCL.Click += new System.EventHandler(this.btnFilterNoOpenCL_Click);
            // 
            // btnConfigFilter
            // 
            this.btnConfigFilter.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnConfigFilter.Image = ((System.Drawing.Image)(resources.GetObject("btnConfigFilter.Image")));
            this.btnConfigFilter.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnConfigFilter.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnConfigFilter.Name = "btnConfigFilter";
            this.btnConfigFilter.Size = new System.Drawing.Size(52, 52);
            this.btnConfigFilter.Text = "Change filter configuration";
            this.btnConfigFilter.Click += new System.EventHandler(this.btnConfigFilter_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 55);
            // 
            // btnStartWebCam
            // 
            this.btnStartWebCam.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnStartWebCam.Image = ((System.Drawing.Image)(resources.GetObject("btnStartWebCam.Image")));
            this.btnStartWebCam.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnStartWebCam.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnStartWebCam.Name = "btnStartWebCam";
            this.btnStartWebCam.Size = new System.Drawing.Size(52, 52);
            this.btnStartWebCam.ToolTipText = "Activate webcam real-time filtering";
            this.btnStartWebCam.Click += new System.EventHandler(this.btnStartWebCam_Click);
            // 
            // btnDeactivateCam
            // 
            this.btnDeactivateCam.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnDeactivateCam.Image = ((System.Drawing.Image)(resources.GetObject("btnDeactivateCam.Image")));
            this.btnDeactivateCam.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnDeactivateCam.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnDeactivateCam.Name = "btnDeactivateCam";
            this.btnDeactivateCam.Size = new System.Drawing.Size(52, 52);
            this.btnDeactivateCam.Text = "toolStripButton3";
            this.btnDeactivateCam.ToolTipText = "Deactivate webcam real-time filtering";
            this.btnDeactivateCam.Click += new System.EventHandler(this.btnDeactivateCam_Click);
            // 
            // lblFps
            // 
            this.lblFps.AutoSize = false;
            this.lblFps.Name = "lblFps";
            this.lblFps.Size = new System.Drawing.Size(60, 52);
            this.lblFps.Text = "0 fps";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 55);
            // 
            // btnCLConfig
            // 
            this.btnCLConfig.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnCLConfig.Image = ((System.Drawing.Image)(resources.GetObject("btnCLConfig.Image")));
            this.btnCLConfig.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnCLConfig.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnCLConfig.Name = "btnCLConfig";
            this.btnCLConfig.Size = new System.Drawing.Size(52, 52);
            this.btnCLConfig.ToolTipText = "Open OpenCL configuration";
            this.btnCLConfig.Click += new System.EventHandler(this.btnCLConfig_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.panel1);
            this.groupBox1.Location = new System.Drawing.Point(12, 58);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(678, 305);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Picture";
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.AutoScroll = true;
            this.panel1.Controls.Add(this.pic);
            this.panel1.Location = new System.Drawing.Point(6, 19);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(666, 280);
            this.panel1.TabIndex = 0;
            // 
            // pic
            // 
            this.pic.Location = new System.Drawing.Point(3, 3);
            this.pic.Name = "pic";
            this.pic.Size = new System.Drawing.Size(150, 116);
            this.pic.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pic.TabIndex = 0;
            this.pic.TabStop = false;
            // 
            // frmMainFilter
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(702, 375);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.toolStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "frmMainFilter";
            this.Text = "CMSoft OpenCL image filtering Case Study";
            this.Load += new System.EventHandler(this.frmMainFilter_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMainFilter_FormClosing);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pic)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton btnOpenFile;
        private System.Windows.Forms.ToolStripButton btnApplyFilter;
        private System.Windows.Forms.ToolStripButton btnSave;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton btnConfigFilter;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton btnStartWebCam;
        private System.Windows.Forms.ToolStripButton btnDeactivateCam;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.PictureBox pic;
        private System.Windows.Forms.ToolStripButton btnCLConfig;
        private System.Windows.Forms.ToolStripLabel lblFps;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton btnFilterNoOpenCL;
        private System.Windows.Forms.ToolStripComboBox cmbWorkDim;
    }
}

