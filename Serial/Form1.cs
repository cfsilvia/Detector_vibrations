using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using LiveCharts;
using LiveCharts.WinForms;
using LiveCharts.Wpf;
using LiveCharts.Definitions.Charts;
using System.IO;

namespace Serial
{
    public partial class Form1 : Form
    {
        SerialPort serialPort;
        private StreamWriter fileWriter;
        string serialBuffer = "";

        ChartValues<double> xValues = new ChartValues<double>();
        ChartValues<double> yValues = new ChartValues<double>();
        ChartValues<double> zValues = new ChartValues<double>();

        public Form1()
        {
            InitializeComponent();
         
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
            cartesianChart.Series = new SeriesCollection
            {
            new LineSeries { Title = "X", Values = xValues, PointGeometry = null,  Fill = System.Windows.Media.Brushes.Transparent  },
            new LineSeries { Title = "Y", Values = yValues, PointGeometry = null, Fill = System.Windows.Media.Brushes.Transparent  },
            new LineSeries { Title = "Z", Values = zValues, PointGeometry = null, Fill = System.Windows.Media.Brushes.Transparent  }
            };

            cartesianChart.AxisX.Add(new Axis
            {
                Title = "Time",
                LabelFormatter = value => "",
                Foreground = System.Windows.Media.Brushes.Black,//Axis label text color
                Separator = new Separator
                {

                    Stroke = System.Windows.Media.Brushes.Gray // 👈 draw axis line
                },
                 ShowLabels = true
            });

            cartesianChart.AxisY.Add(new Axis
            {
                Title =  "Acceleration (m/s²)",
                MinValue = -20,
                MaxValue = 20,
                Foreground = System.Windows.Media.Brushes.Black,
                Separator = new Separator
                {
                    Stroke = System.Windows.Media.Brushes.Gray // 👈 draw axis line
                },
                ShowLabels = true
            });
            cartesianChart.LegendLocation = LegendLocation.Right;
            
        }


        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if (serialPort == null || !serialPort.IsOpen)
            {
                try
                {
                    serialPort = new SerialPort("COM3", 9600); 
                    serialPort.DataReceived += SerialPort_DataReceived;
                    fileWriter = new StreamWriter("D:\\Silvia\\arduino_detector\\Data\\data.csv", true, Encoding.UTF8);
                    serialPort.Open();
                   
                    timer1.Start();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: " + ex.Message);
                }
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            serialBuffer += serialPort.ReadExisting();
        }

        private void buttonStop_Click(object sender, EventArgs e)
        {
           
            serialPort.Close();
            fileWriter.Close();
        }

        private void cartesianChart_ChildChanged(object sender, System.Windows.Forms.Integration.ChildChangedEventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {

            if (serialBuffer.Contains("\n"))
            {
                string[] lines = serialBuffer.Split('\n');
                foreach (var line in lines)
                {
                   

                    var parts = line.Trim().Split(',');
                    if (parts.Length == 4 &&
                        double.TryParse(parts[1], out double x) &&
                        double.TryParse(parts[2], out double y) &&
                        double.TryParse(parts[3], out double z))
                    {
                        xValues.Add(x);
                        yValues.Add(y);
                        zValues.Add(z);

                        if (xValues.Count > 50)
                        {
                            xValues.RemoveAt(0);
                            yValues.RemoveAt(0);
                            zValues.RemoveAt(0);
                        }

                        cartesianChart.Refresh();

                        // write the line in the writer
                        fileWriter.WriteLine(line);
                        fileWriter.Flush();
                    }
                }

                serialBuffer = "";
            }
        }
    }
}
