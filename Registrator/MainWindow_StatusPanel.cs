using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using System.Runtime.InteropServices;
using System.Collections;
using System.IO;
using System.Threading;
using System.Runtime.Serialization.Formatters.Binary;

namespace Registrator
{
    public partial class MainWindow
    {

        void connect_player_errors()
        {
            m_doc.ErrorHandler += PlayerErrorAquired;
            m_doc.ResetErrorsHandler += PlayerResetErrors;
        }

        void PlayerErrorAquired(object sender,ErrorEvent error)
        {
            errorTextCtrl.Text = error.ErrorText;
            errorLabel.Visible = true;
            errorTextCtrl.Visible = true;
        }
        void PlayerResetErrors(object sender, EventArgs e)
        {
            errorTextCtrl.Text = "";
            errorLabel.Visible = false;
            errorTextCtrl.Visible = false;
        }

        void connect_player_state_event()
        {
            m_doc.PlayerStateEventHandler += PlayerPanelStateChanged;
            m_doc.MovieFilesLoading += MovieFilesLoadingProgress;
        }

        void connect_pd_dispatcher_events()
        {
            m_doc.PositionDetector.PD_ConnectionStateChanged += PD_ConnectionStateChanged;
            m_doc.PositionDetector.PD_ConnectionError += PD_ErrorsAquire;
            
            if (m_doc.PositionDetector.PD_State)
                PD_ConnectionStateChanged(true, "");
            else
            {
                PD_ConnectionStateChanged(false, "");
                var error = m_doc.PositionDetector.Error;
                if (error != null && error.Length != 0)
                    PD_ErrorsAquire(error);
            }

        }

        void connect_grabber_dispatcher_events()
        {
            m_doc.Grabber.GrabberStateChanged += grabberDispatcherStateChanged;
            m_doc.Grabber.GrabberErrorAquired += grabberDispatcherErrorsAquire;

        }

        void MovieFilesLoadingProgress(Int32 value)
        {
            bool change_visibility = false;
            bool visibility = false;
            if(value == 0){
                change_visibility = true;
                visibility = true;
            }
            if(value == -1){
                value = 100;
                change_visibility = true;
            }

            if (change_visibility)
            {
                toolStripProgressBar1.Enabled = visibility;
                toolStripProgressBar1.Visible = visibility;
            }

            toolStripProgressBar1.Value = value;
        }


        void PlayerPanelStateChanged(object sender, PlayerStateEvent ev)
        {
            if (InvokeRequired)
                statusPanel.Invoke(new EventHandler(delegate
                {
                    if (ev.state == PlayerPanel.PlayerState.BUSY)
                    {
                        statusPanel.Items["PlayerPanelStatusLabel"].Text = "Wait until the operation complited...";
                        statusPanel.Items["PlayerPanelStatusLabel"].Visible = true;
                    }
                    else
                    {
                        statusPanel.Items["PlayerPanelStatusLabel"].Visible = false;
                        statusPanel.Items["PlayerPanelStatusLabel"].Text = "";
                    }
                }
               ));
            else
            {
                if (ev.state == PlayerPanel.PlayerState.BUSY)
                {
                    statusPanel.Items["PlayerPanelStatusLabel"].Text = "Wait until the operation complited...";
                    statusPanel.Items["PlayerPanelStatusLabel"].Visible = true;
                }
                else
                {
                    statusPanel.Items["PlayerPanelStatusLabel"].Visible = false;
                    statusPanel.Items["PlayerPanelStatusLabel"].Text = "";
                }

            }

        }



        void grabberDispatcherStateChanged(GrabberState state, string info)
        {
            statusPanel.Invoke(new EventHandler(delegate
            {
                string text = "Подключено";
                if (state == GrabberState.NONE ||
                    state == GrabberState.ERROR ||
                    state == GrabberState.INITED
                    )
                    text = "подключение не установлено";
                statusPanel.Items["camStatus"].Text = text;
            }
           ));

        }
        void grabberDispatcherErrorsAquire(string error)
        {
            statusPanel.Invoke(new EventHandler(delegate
            {
                statusPanel.Items["cameraErrorsStatus"].Visible = true;
                statusPanel.Items["cameraErrorsStatus"].Text = error;
            }
                                         ));

        }


        void PD_ConnectionStateChanged(bool state, string info)
        {
            statusPanel.Invoke(new EventHandler(delegate
            {
                string text = "Подключено";
                if (!state)
                    text = "подключение не установлено";
                statusPanel.Items["PD_Status"].Text = text;
            }
                                         ));

        }

        void PD_ErrorsAquire(string error)
        {
            statusPanel.Invoke(new EventHandler(delegate
            {
                var item = statusPanel.Items["PDErrosStatus"];
                item.Visible = true;
                item.Text = error;
            }
                                         ));

        }

    }
}
