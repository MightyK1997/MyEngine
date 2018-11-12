using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Launcher
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private const string fileName = "data/controllerSettings.dat";
        public enum ControllerKeyCodes
        {
            DPAD_UP = 0x0001,
            DPAD_DOWN = 0x0002,
            DPAD_LEFT = 0x0004,
            DPAD_RIGHT = 0x0008,
            START = 0x0010,
            BACK = 0x0020,
            LEFT_THUMB = 0x0040,
            RIGHT_THUMB = 0x0080,
            LEFT_SHOULDER = 0x0100,
            RIGHT_SHOULDER = 0x0200,
            A = 0x1000,
            B = 0x2000,
            X = 0x4000,
            Y = 0x8000,
        };
        public MainWindow()
        {
            InitializeComponent();
            List<ControllerKeyCodes> m_AllButtonsList =
                Enum.GetValues(typeof(ControllerKeyCodes)).Cast<ControllerKeyCodes>().ToList();

            cbxA.ItemsSource = m_AllButtonsList;
            cbxB.ItemsSource = m_AllButtonsList;
            cbxX.ItemsSource = m_AllButtonsList;
            cbxY.ItemsSource = m_AllButtonsList;
            cbxDUp.ItemsSource = m_AllButtonsList;
            cbxDDown.ItemsSource = m_AllButtonsList;
            cbxDLeft.ItemsSource = m_AllButtonsList;
            cbxDRight.ItemsSource = m_AllButtonsList;
            cbxStart.ItemsSource = m_AllButtonsList;
            cbxBack.ItemsSource = m_AllButtonsList;
            cbxLTS.ItemsSource = m_AllButtonsList;
            cbxRTS.ItemsSource = m_AllButtonsList;
            cbxLB.ItemsSource = m_AllButtonsList;
            cbxRB.ItemsSource = m_AllButtonsList;
        }

        private void btnOkbutton_Click(object sender, RoutedEventArgs e)
        {
            using (BinaryWriter writer= new BinaryWriter(File.Open(fileName, FileMode.Create)))
            {
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxA.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxB.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxX.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxY.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxDUp.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxDDown.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxDLeft.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxDRight.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxStart.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxBack.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxLTS.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxRTS.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxLB.SelectedItem.ToString())));
                writer.Write(Convert.ToInt32(Enum.Parse(typeof(ControllerKeyCodes), cbxRB.SelectedItem.ToString())));
            }

            this.Close();
        }

        private void btnCancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
