using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AssetPackager
{
    public partial class AssetPackager : Form
    {
        public AssetPackager()
        {
            InitializeComponent();
        }

        private List<FileStream> _fileStreams = new List<FileStream>();
        private void loadFiles_Click(object sender, EventArgs e)
        {
            if(sender == null)
                return;

            
            OpenFileDialog openFileDialog = new OpenFileDialog
            {
                InitialDirectory = "C:\\Git\\GameEngine\\GameEngine\\Resources",
                Filter = "All files (*.*)|*.*",
                FilterIndex = 1,
                Multiselect = true,
                RestoreDirectory = true
            };

            if (openFileDialog.ShowDialog() != DialogResult.OK)
                return;
            try
            {
                foreach (String file in openFileDialog.FileNames)
                {
                    _fileStreams.Add(File.Open(file, FileMode.Append));
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);
            }
        }
    }
}
