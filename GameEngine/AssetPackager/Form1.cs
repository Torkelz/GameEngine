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

            createFiles.Enabled = false;
        }

        private List<String> filePaths = new List<String>();
        private List<byte[]> dataz = new List<byte[]>(); 

        private void loadFiles_OnClick(object sender, EventArgs e)
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
                    filePaths.Add(file);
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);
            }

            foreach (var file in filePaths)
            {
                try
                {
                    if (file == null)
                    {
                        continue;
                    }
                    using (FileStream fs = File.OpenRead(file))
                    {
                        int length = (int)fs.Length;
                        byte[] data = new byte[length];
                        fs.Read(data, 0, length);
                        dataz.Add(data);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
            createFiles.Enabled = true;

        }

        private void createPackage_OnClick(object sender, EventArgs e)
        {
            if (sender == null)
                return;

            try
            {
                using (FileStream fs = new FileStream("C:\\Git\\GameEngine\\GameEngine\\Resources\\ToolTip.mtxp", FileMode.CreateNew))//File.Create("C:\\Git\\GameEngine\\GameEngine\\Resources\\ToolTip.mtxp"))
                {
                    using (BinaryWriter writer = new BinaryWriter(fs, Encoding.ASCII))
                    {
                        //Header creation
                        writer.Write(System.Text.Encoding.ASCII.GetBytes("# Header\n"));
                        int position = 0;
                        for (int i = 0; i < filePaths.Count(); ++i)
                        {
                            writer.Write(System.Text.Encoding.ASCII.GetBytes(filePaths[i] + "\t"));
                            writer.Write(System.Text.Encoding.ASCII.GetBytes(position.ToString() + "\t"));
                            int size = (int)dataz[i].Length;
                            writer.Write(System.Text.Encoding.ASCII.GetBytes(size.ToString() + "\n"));
                            position += size;
                        }
                        writer.Write(System.Text.Encoding.ASCII.GetBytes("# End of Header\n"));
                        writer.Write((int)writer.BaseStream.Length + 4);

                        //Fill with data
                        for (int i = 0; i < filePaths.Count(); ++i)
                        {
                            writer.Write(dataz[i]);
                        }
                    }
                }
                MessageBox.Show("Done");
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: File already exists: " + ex.Message);
            }
        }
    }
}
