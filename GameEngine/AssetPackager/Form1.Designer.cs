namespace AssetPackager
{
    partial class AssetPackager
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
            this.loadFiles = new System.Windows.Forms.Button();
            this.createFiles = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // loadFiles
            // 
            this.loadFiles.Location = new System.Drawing.Point(345, 281);
            this.loadFiles.Name = "loadFiles";
            this.loadFiles.Size = new System.Drawing.Size(75, 23);
            this.loadFiles.TabIndex = 0;
            this.loadFiles.Text = "Load files...";
            this.loadFiles.UseVisualStyleBackColor = true;
            this.loadFiles.Click += new System.EventHandler(this.loadFiles_Click);
            // 
            // createFiles
            // 
            this.createFiles.Location = new System.Drawing.Point(514, 280);
            this.createFiles.Name = "createFiles";
            this.createFiles.Size = new System.Drawing.Size(97, 23);
            this.createFiles.TabIndex = 1;
            this.createFiles.Text = "Create package";
            this.createFiles.UseVisualStyleBackColor = true;
            // 
            // AssetPackager
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(784, 461);
            this.Controls.Add(this.createFiles);
            this.Controls.Add(this.loadFiles);
            this.Name = "AssetPackager";
            this.Text = "Asset Packager";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button loadFiles;
        private System.Windows.Forms.Button createFiles;
    }
}

