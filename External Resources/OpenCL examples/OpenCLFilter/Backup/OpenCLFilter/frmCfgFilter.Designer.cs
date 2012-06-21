namespace OpenCLFilter
{
    partial class frmCfgFilter
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmCfgFilter));
            this.label1 = new System.Windows.Forms.Label();
            this.tabCtrl = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.gbR = new System.Windows.Forms.GroupBox();
            this.txtRCode = new System.Windows.Forms.TextBox();
            this.lblAmpR = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.btnNormR = new System.Windows.Forms.Button();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.gbG = new System.Windows.Forms.GroupBox();
            this.txtGCode = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.lblAmpG = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.btnNormG = new System.Windows.Forms.Button();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.gbB = new System.Windows.Forms.GroupBox();
            this.txtBCode = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.lblAmpB = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.btnNormB = new System.Windows.Forms.Button();
            this.btnCopy = new System.Windows.Forms.Button();
            this.btnClear = new System.Windows.Forms.Button();
            this.tabCtrl.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.gbR.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.gbG.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.gbB.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(397, 112);
            this.label1.TabIndex = 0;
            this.label1.Text = resources.GetString("label1.Text");
            // 
            // tabCtrl
            // 
            this.tabCtrl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tabCtrl.Controls.Add(this.tabPage1);
            this.tabCtrl.Controls.Add(this.tabPage3);
            this.tabCtrl.Controls.Add(this.tabPage2);
            this.tabCtrl.Location = new System.Drawing.Point(12, 129);
            this.tabCtrl.Name = "tabCtrl";
            this.tabCtrl.SelectedIndex = 0;
            this.tabCtrl.Size = new System.Drawing.Size(397, 314);
            this.tabCtrl.TabIndex = 2;
            this.tabCtrl.Leave += new System.EventHandler(this.txtCode_Leave);
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.gbR);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(389, 288);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Red filter";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // gbR
            // 
            this.gbR.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbR.Controls.Add(this.txtRCode);
            this.gbR.Controls.Add(this.lblAmpR);
            this.gbR.Controls.Add(this.label8);
            this.gbR.Controls.Add(this.label2);
            this.gbR.Controls.Add(this.btnNormR);
            this.gbR.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbR.ForeColor = System.Drawing.Color.Red;
            this.gbR.Location = new System.Drawing.Point(6, 6);
            this.gbR.Name = "gbR";
            this.gbR.Size = new System.Drawing.Size(372, 276);
            this.gbR.TabIndex = 2;
            this.gbR.TabStop = false;
            this.gbR.Text = "Red filter";
            // 
            // txtRCode
            // 
            this.txtRCode.Location = new System.Drawing.Point(83, 240);
            this.txtRCode.Name = "txtRCode";
            this.txtRCode.Size = new System.Drawing.Size(283, 20);
            this.txtRCode.TabIndex = 5;
            this.txtRCode.Leave += new System.EventHandler(this.txtCode_Leave);
            // 
            // lblAmpR
            // 
            this.lblAmpR.AutoSize = true;
            this.lblAmpR.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblAmpR.Location = new System.Drawing.Point(247, 206);
            this.lblAmpR.Name = "lblAmpR";
            this.lblAmpR.Size = new System.Drawing.Size(16, 15);
            this.lblAmpR.TabIndex = 4;
            this.lblAmpR.Text = "1";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 243);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(71, 13);
            this.label8.TabIndex = 4;
            this.label8.Text = "Filter code:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(144, 206);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(97, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Filter amplitude:";
            // 
            // btnNormR
            // 
            this.btnNormR.Location = new System.Drawing.Point(6, 201);
            this.btnNormR.Name = "btnNormR";
            this.btnNormR.Size = new System.Drawing.Size(132, 23);
            this.btnNormR.TabIndex = 3;
            this.btnNormR.Text = "Normalize filter";
            this.btnNormR.UseVisualStyleBackColor = true;
            this.btnNormR.Click += new System.EventHandler(this.btnNorm_Click);
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.gbG);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(389, 288);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Green filter";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // gbG
            // 
            this.gbG.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbG.Controls.Add(this.txtGCode);
            this.gbG.Controls.Add(this.label9);
            this.gbG.Controls.Add(this.lblAmpG);
            this.gbG.Controls.Add(this.label7);
            this.gbG.Controls.Add(this.btnNormG);
            this.gbG.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbG.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.gbG.Location = new System.Drawing.Point(6, 6);
            this.gbG.Name = "gbG";
            this.gbG.Size = new System.Drawing.Size(372, 279);
            this.gbG.TabIndex = 4;
            this.gbG.TabStop = false;
            this.gbG.Text = "Green filter";
            // 
            // txtGCode
            // 
            this.txtGCode.Location = new System.Drawing.Point(83, 240);
            this.txtGCode.Name = "txtGCode";
            this.txtGCode.Size = new System.Drawing.Size(283, 20);
            this.txtGCode.TabIndex = 7;
            this.txtGCode.Leave += new System.EventHandler(this.txtCode_Leave);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(6, 243);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(71, 13);
            this.label9.TabIndex = 6;
            this.label9.Text = "Filter code:";
            // 
            // lblAmpG
            // 
            this.lblAmpG.AutoSize = true;
            this.lblAmpG.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblAmpG.Location = new System.Drawing.Point(247, 206);
            this.lblAmpG.Name = "lblAmpG";
            this.lblAmpG.Size = new System.Drawing.Size(16, 15);
            this.lblAmpG.TabIndex = 4;
            this.lblAmpG.Text = "1";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(144, 206);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(97, 13);
            this.label7.TabIndex = 4;
            this.label7.Text = "Filter amplitude:";
            // 
            // btnNormG
            // 
            this.btnNormG.Location = new System.Drawing.Point(6, 201);
            this.btnNormG.Name = "btnNormG";
            this.btnNormG.Size = new System.Drawing.Size(132, 23);
            this.btnNormG.TabIndex = 3;
            this.btnNormG.Text = "Normalize filter";
            this.btnNormG.UseVisualStyleBackColor = true;
            this.btnNormG.Click += new System.EventHandler(this.btnNorm_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.gbB);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(389, 288);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Blue filter";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // gbB
            // 
            this.gbB.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbB.Controls.Add(this.txtBCode);
            this.gbB.Controls.Add(this.label10);
            this.gbB.Controls.Add(this.lblAmpB);
            this.gbB.Controls.Add(this.label5);
            this.gbB.Controls.Add(this.btnNormB);
            this.gbB.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbB.ForeColor = System.Drawing.Color.Blue;
            this.gbB.Location = new System.Drawing.Point(6, 6);
            this.gbB.Name = "gbB";
            this.gbB.Size = new System.Drawing.Size(377, 276);
            this.gbB.TabIndex = 3;
            this.gbB.TabStop = false;
            this.gbB.Text = "Blue filter";
            // 
            // txtBCode
            // 
            this.txtBCode.Location = new System.Drawing.Point(83, 240);
            this.txtBCode.Name = "txtBCode";
            this.txtBCode.Size = new System.Drawing.Size(283, 20);
            this.txtBCode.TabIndex = 7;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(6, 243);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(71, 13);
            this.label10.TabIndex = 6;
            this.label10.Text = "Filter code:";
            // 
            // lblAmpB
            // 
            this.lblAmpB.AutoSize = true;
            this.lblAmpB.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblAmpB.Location = new System.Drawing.Point(247, 206);
            this.lblAmpB.Name = "lblAmpB";
            this.lblAmpB.Size = new System.Drawing.Size(16, 15);
            this.lblAmpB.TabIndex = 4;
            this.lblAmpB.Text = "1";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(144, 206);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(97, 13);
            this.label5.TabIndex = 4;
            this.label5.Text = "Filter amplitude:";
            // 
            // btnNormB
            // 
            this.btnNormB.Location = new System.Drawing.Point(6, 201);
            this.btnNormB.Name = "btnNormB";
            this.btnNormB.Size = new System.Drawing.Size(132, 23);
            this.btnNormB.TabIndex = 3;
            this.btnNormB.Text = "Normalize filter";
            this.btnNormB.UseVisualStyleBackColor = true;
            this.btnNormB.Click += new System.EventHandler(this.btnNorm_Click);
            // 
            // btnCopy
            // 
            this.btnCopy.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnCopy.Location = new System.Drawing.Point(12, 449);
            this.btnCopy.Name = "btnCopy";
            this.btnCopy.Size = new System.Drawing.Size(203, 40);
            this.btnCopy.TabIndex = 3;
            this.btnCopy.Text = "Copy red filter to green and blue filters";
            this.btnCopy.UseVisualStyleBackColor = true;
            this.btnCopy.Click += new System.EventHandler(this.btnCopy_Click);
            // 
            // btnClear
            // 
            this.btnClear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnClear.Location = new System.Drawing.Point(221, 449);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(188, 40);
            this.btnClear.TabIndex = 3;
            this.btnClear.Text = "Clear all";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // frmCfgFilter
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(421, 492);
            this.Controls.Add(this.btnClear);
            this.Controls.Add(this.btnCopy);
            this.Controls.Add(this.tabCtrl);
            this.Controls.Add(this.label1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "frmCfgFilter";
            this.Text = "Filter Configuration";
            this.Load += new System.EventHandler(this.frmCfgFilter_Load);
            this.tabCtrl.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.gbR.ResumeLayout(false);
            this.gbR.PerformLayout();
            this.tabPage3.ResumeLayout(false);
            this.gbG.ResumeLayout(false);
            this.gbG.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.gbB.ResumeLayout(false);
            this.gbB.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TabControl tabCtrl;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.GroupBox gbR;
        private System.Windows.Forms.Label lblAmpR;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnNormR;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.GroupBox gbB;
        private System.Windows.Forms.Label lblAmpB;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button btnNormB;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.GroupBox gbG;
        private System.Windows.Forms.Label lblAmpG;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button btnNormG;
        private System.Windows.Forms.Button btnCopy;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.TextBox txtRCode;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox txtGCode;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox txtBCode;
        private System.Windows.Forms.Label label10;
    }
}