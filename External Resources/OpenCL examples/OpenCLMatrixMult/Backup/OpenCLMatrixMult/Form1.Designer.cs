namespace OpenCLMatrixMult
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.btnMultMatrix1 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.numM = new System.Windows.Forms.NumericUpDown();
            this.numQ = new System.Windows.Forms.NumericUpDown();
            this.numR = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.lblMultNoLocals = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.lblMultLocals = new System.Windows.Forms.Label();
            this.lblCPUtime = new System.Windows.Forms.Label();
            this.lblErrNoLocals = new System.Windows.Forms.Label();
            this.lblErrLocals = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.numM)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numQ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numR)).BeginInit();
            this.SuspendLayout();
            // 
            // btnMultMatrix1
            // 
            this.btnMultMatrix1.Location = new System.Drawing.Point(12, 176);
            this.btnMultMatrix1.Name = "btnMultMatrix1";
            this.btnMultMatrix1.Size = new System.Drawing.Size(306, 30);
            this.btnMultMatrix1.TabIndex = 0;
            this.btnMultMatrix1.Text = "Perform comparisons and check accuracy";
            this.btnMultMatrix1.UseVisualStyleBackColor = true;
            this.btnMultMatrix1.Click += new System.EventHandler(this.btnMultMatrix1_Click);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(310, 110);
            this.label1.TabIndex = 1;
            this.label1.Text = resources.GetString("label1.Text");
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 138);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(17, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "P:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(119, 138);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(18, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Q:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(226, 138);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(18, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "R:";
            // 
            // numM
            // 
            this.numM.Increment = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.numM.Location = new System.Drawing.Point(46, 136);
            this.numM.Maximum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.numM.Minimum = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.numM.Name = "numM";
            this.numM.Size = new System.Drawing.Size(67, 20);
            this.numM.TabIndex = 3;
            this.numM.Value = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.numM.Leave += new System.EventHandler(this.numM_Leave);
            // 
            // numN
            // 
            this.numQ.Increment = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.numQ.Location = new System.Drawing.Point(143, 136);
            this.numQ.Maximum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.numQ.Minimum = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.numQ.Name = "numN";
            this.numQ.Size = new System.Drawing.Size(67, 20);
            this.numQ.TabIndex = 3;
            this.numQ.Value = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.numQ.Leave += new System.EventHandler(this.numM_Leave);
            // 
            // numP
            // 
            this.numR.Increment = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.numR.Location = new System.Drawing.Point(249, 136);
            this.numR.Maximum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.numR.Minimum = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.numR.Name = "numP";
            this.numR.Size = new System.Drawing.Size(67, 20);
            this.numR.TabIndex = 3;
            this.numR.Value = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.numR.Leave += new System.EventHandler(this.numM_Leave);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 222);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(159, 13);
            this.label5.TabIndex = 4;
            this.label5.Text = "OpenCL multiply time (no locals):";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(12, 247);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(144, 13);
            this.label6.TabIndex = 4;
            this.label6.Text = "OpenCL multiply time (locals):";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(12, 293);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(147, 13);
            this.label7.TabIndex = 4;
            this.label7.Text = "Mean square error (no locals):";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(12, 315);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(132, 13);
            this.label8.TabIndex = 4;
            this.label8.Text = "Mean square error (locals):";
            // 
            // lblMultNoLocals
            // 
            this.lblMultNoLocals.AutoSize = true;
            this.lblMultNoLocals.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblMultNoLocals.Location = new System.Drawing.Point(175, 222);
            this.lblMultNoLocals.Name = "lblMultNoLocals";
            this.lblMultNoLocals.Size = new System.Drawing.Size(15, 15);
            this.lblMultNoLocals.TabIndex = 5;
            this.lblMultNoLocals.Text = "0";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(12, 270);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(136, 13);
            this.label9.TabIndex = 4;
            this.label9.Text = "Double precision CPU time:";
            // 
            // lblMultLocals
            // 
            this.lblMultLocals.AutoSize = true;
            this.lblMultLocals.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblMultLocals.Location = new System.Drawing.Point(175, 247);
            this.lblMultLocals.Name = "lblMultLocals";
            this.lblMultLocals.Size = new System.Drawing.Size(15, 15);
            this.lblMultLocals.TabIndex = 5;
            this.lblMultLocals.Text = "0";
            // 
            // lblCPUtime
            // 
            this.lblCPUtime.AutoSize = true;
            this.lblCPUtime.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblCPUtime.Location = new System.Drawing.Point(175, 270);
            this.lblCPUtime.Name = "lblCPUtime";
            this.lblCPUtime.Size = new System.Drawing.Size(15, 15);
            this.lblCPUtime.TabIndex = 5;
            this.lblCPUtime.Text = "0";
            // 
            // lblErrNoLocals
            // 
            this.lblErrNoLocals.AutoSize = true;
            this.lblErrNoLocals.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblErrNoLocals.Location = new System.Drawing.Point(175, 293);
            this.lblErrNoLocals.Name = "lblErrNoLocals";
            this.lblErrNoLocals.Size = new System.Drawing.Size(15, 15);
            this.lblErrNoLocals.TabIndex = 5;
            this.lblErrNoLocals.Text = "0";
            // 
            // lblErrLocals
            // 
            this.lblErrLocals.AutoSize = true;
            this.lblErrLocals.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblErrLocals.Location = new System.Drawing.Point(175, 315);
            this.lblErrLocals.Name = "lblErrLocals";
            this.lblErrLocals.Size = new System.Drawing.Size(15, 15);
            this.lblErrLocals.TabIndex = 5;
            this.lblErrLocals.Text = "0";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(334, 373);
            this.Controls.Add(this.lblErrLocals);
            this.Controls.Add(this.lblErrNoLocals);
            this.Controls.Add(this.lblCPUtime);
            this.Controls.Add(this.lblMultLocals);
            this.Controls.Add(this.lblMultNoLocals);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.numR);
            this.Controls.Add(this.numQ);
            this.Controls.Add(this.numM);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnMultMatrix1);
            this.Name = "Form1";
            this.Text = "Matrix multiplication tutorial";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.numM)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numQ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numR)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnMultMatrix1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown numM;
        private System.Windows.Forms.NumericUpDown numQ;
        private System.Windows.Forms.NumericUpDown numR;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label lblMultNoLocals;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label lblMultLocals;
        private System.Windows.Forms.Label lblCPUtime;
        private System.Windows.Forms.Label lblErrNoLocals;
        private System.Windows.Forms.Label lblErrLocals;
    }
}

