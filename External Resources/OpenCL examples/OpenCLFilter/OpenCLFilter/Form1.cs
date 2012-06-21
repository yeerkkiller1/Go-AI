using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AForge.Video;
using AForge.Video.DirectShow;

namespace OpenCLFilter
{
    public partial class frmMainFilter : Form
    {
        public frmMainFilter()
        {
            InitializeComponent();
        }

        frmCfgFilter frmFilter;
        private void btnConfigFilter_Click(object sender, EventArgs e)
        {
            if (frmFilter == null || frmFilter.IsDisposed)
                frmFilter = new frmCfgFilter();

            frmFilter.Show();
        }

        #region Open and store file, apply filter to bitmap
        Bitmap bmp;
        ImageData imgDt;
        private void btnOpenFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "Pictures|*.jpg";

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    bmp = new Bitmap(ofd.FileName);
                    imgDt = new ImageData(bmp);
                    pic.Image = bmp;
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString(), this.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
        
        private void btnApplyFilter_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Stopwatch sWatch = new System.Diagnostics.Stopwatch();
            sWatch.Start();

            if (frmFilter == null || frmFilter.IsDisposed)
                frmFilter = new frmCfgFilter();

            frmFilter.Show();

            float[] filter = frmFilter.GetFilters();

            CLFilter.ApplyFilter(imgDt, filter, true, cmbWorkDim.SelectedIndex == 1);

            bmp = imgDt.GetStoredBitmap(bmp);
            pic.Image = bmp;

            sWatch.Stop();
            lblFps.Text = sWatch.Elapsed.ToString();
        }

        private void btnFilterNoOpenCL_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Stopwatch sWatch = new System.Diagnostics.Stopwatch();
            sWatch.Start();

            if (frmFilter == null || frmFilter.IsDisposed)
                frmFilter = new frmCfgFilter();

            frmFilter.Show();

            float[] filter = frmFilter.GetFilters();

            CLFilter.ApplyFilter(imgDt, filter, false, false);

            bmp = imgDt.GetStoredBitmap(bmp);
            pic.Image = bmp;

            sWatch.Stop();
            lblFps.Text = sWatch.Elapsed.ToString();

        }
        #endregion



        private void btnCLConfig_Click(object sender, EventArgs e)
        {
            OpenCLTemplate.frmCLInfo frmCL = new OpenCLTemplate.frmCLInfo();
            frmCL.ShowDialog();

        }

        #region Webcam

        VideoCaptureDevice vcd;
        ImageData ImgDtWebCam;
        private void btnStartWebCam_Click(object sender, EventArgs e)
        {
            if (frmFilter == null || frmFilter.IsDisposed)
                frmFilter = new frmCfgFilter();

            frmFilter.Show();
            filterWebCam = frmFilter.GetFilters();


            FilterInfoCollection devs = new FilterInfoCollection(FilterCategory.VideoInputDevice);
            delegRefreshPic = RefreshPic;
            vcd = new VideoCaptureDevice(devs[0].MonikerString);
            vcd.NewFrame += new NewFrameEventHandler(vcd_NewFrame);
            vcd.DesiredFrameSize = new Size(600, 400);
            vcd.Start();
        }

        System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
        bool processando = false;
        int nFrames = 0; double fRate = 1;
        float[] filterWebCam;
        void vcd_NewFrame(object sender, NewFrameEventArgs eventArgs)
        {
            try
            {
                if (!processando)
                {
                    processando = true;

                    bmp = (Bitmap)eventArgs.Frame.Clone();
                    if (ImgDtWebCam == null)
                        ImgDtWebCam = new ImageData(bmp);
                    else ImgDtWebCam.ReadBmp(bmp);

                    //Filter
                    if (!frmFilter.IsDisposed) filterWebCam = frmFilter.GetFilters();

                    CLFilter.ApplyFilter(ImgDtWebCam, filterWebCam, true, WorkDim2);
                    bmp = ImgDtWebCam.GetStoredBitmap(bmp);


                    if (!sw.IsRunning) sw.Start();


                    nFrames++;
                    fRate = nFrames / sw.Elapsed.TotalSeconds;

                    this.Invoke(delegRefreshPic);

                    processando = false;
                }
            }
            catch (Exception ex)
            {
                this.Text = ex.ToString();
            }

        }

        private delegate void voidFunc();
        voidFunc delegRefreshPic;

        private void RefreshPic()
        {
            pic.Image = bmp;
            pic.Refresh();
            lblFps.Text = Math.Round(fRate, 3).ToString();
        }

        private void btnDeactivateCam_Click(object sender, EventArgs e)
        {
            sw.Stop(); sw.Reset();
            nFrames = 0; fRate = 1;
            CloseVIdeo();
        }

        private void CloseVIdeo()
        {
            if (vcd != null)
            {
                if (vcd.IsRunning)
                {
                    vcd.SignalToStop();
                    vcd = null;
                }


            }
        }
        
        private void frmMainFilter_FormClosing(object sender, FormClosingEventArgs e)
        {
            CloseVIdeo();
        }
        #endregion

        private void frmMainFilter_Load(object sender, EventArgs e)
        {
            cmbWorkDim.SelectedIndex = 1;
        }

        bool WorkDim2 = true;
        private void cmbWorkDim_Click(object sender, EventArgs e)
        {
        }

        private void cmbWorkDim_SelectedIndexChanged(object sender, EventArgs e)
        {
            WorkDim2 = (cmbWorkDim.SelectedIndex == 1);
        }





    }
}
