using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Timers;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
namespace ChatServer
{
    struct position
    {
        public int latitude;
        public int longitude;
        public position(int latitudep,int longitudep)
        {
            latitude = latitudep;
            longitude = longitudep;
        }
    }
    class virtualworld
    { 
        public static Dictionary<position, HashSet<KcpChannel>> Cells;
        static virtualworld()
        {
            Cells = new Dictionary<position, HashSet<KcpChannel>>();
        }
    }
    class KcpChannelManager
    {
        public static Dictionary<String, KcpChannel> OnchannelReceivedatacallbackmap;
        static KcpChannelManager()
        {
            OnchannelReceivedatacallbackmap = new Dictionary<string, KcpChannel>();
        }
    }
    class KcpChannel
    {
        public const int idlength = 32;  
        String channelid;
        public KcpClient mkcpclient;
        position currentposition;
        position[] potentialsynccell = new position[9];
        void fleshpotentialsyncell()
        {
            potentialsynccell[0].latitude = currentposition.latitude/1000 + 1;//up
            potentialsynccell[0].longitude = currentposition.longitude / 1000;

            potentialsynccell[1].latitude = currentposition.latitude / 1000 - 1;//down
            potentialsynccell[1].longitude = currentposition.longitude / 1000;

            potentialsynccell[2].latitude = currentposition.latitude / 1000;
            potentialsynccell[2].longitude = currentposition.longitude / 1000 - 1;//left

            potentialsynccell[3].latitude = currentposition.latitude / 1000;
            potentialsynccell[3].longitude = currentposition.longitude / 1000 + 1;//right

            potentialsynccell[4].latitude = currentposition.latitude / 1000 + 1;//up
            potentialsynccell[4].longitude = currentposition.longitude / 1000 + 1;//right

            potentialsynccell[5].latitude = currentposition.latitude / 1000 - 1;//down
            potentialsynccell[5].longitude = currentposition.longitude / 1000 + 1;//right

            potentialsynccell[6].latitude = currentposition.latitude / 1000 - 1;//down
            potentialsynccell[6].longitude = currentposition.longitude / 1000 - 1;//left

            potentialsynccell[7].latitude = currentposition.latitude / 1000 + 1;//up
            potentialsynccell[7].longitude = currentposition.longitude / 1000 - 1;//left

            potentialsynccell[8].latitude = currentposition.latitude / 1000;//center
            potentialsynccell[8].longitude = currentposition.longitude / 1000;//center
        }
        HashSet<KcpChannel> findallpeopelatrange(int radius)
        {
            HashSet<KcpChannel> peopleatrange = new HashSet<KcpChannel>();
            for (int i = 0; i < potentialsynccell.Length;i++)
            {
                bool b = virtualworld.Cells.ContainsKey(potentialsynccell[i]);
                if (b)
                {
                    HashSet<KcpChannel> outpeople;
                    virtualworld.Cells.TryGetValue(potentialsynccell[i], out outpeople);
                    foreach (var p in outpeople)
                    {
                        if (p == this)
                        {
                            continue;
                        }
                        int delta_latitude = p.currentposition.latitude - currentposition.latitude;
                        int delta_longitude = p.currentposition.longitude - currentposition.longitude;
                        int square = (delta_latitude * delta_latitude) + (delta_longitude * delta_longitude);
                        if ( square < radius* radius )
                        { 
                           peopleatrange.Add(p);
                        }
                    }
                }
            }
            return peopleatrange;
        }
        void sentpeopleatrangetome()
        {
            HashSet<KcpChannel> peopleatrange = findallpeopelatrange(1000);
            foreach (var p in peopleatrange)
            {
                JObject jObject = new JObject();
                jObject.Add("Command", 1);
                jObject.Add("channelid", p.channelid);
                jObject.Add("latitude", p.currentposition.latitude);
                jObject.Add("longitude", p.currentposition.longitude);
                send(jObject.ToString());
            }
        }
        public KcpChannel(String channelidp, KcpClient kcpclientp )
        {
            channelid = channelidp;
            mkcpclient = kcpclientp;
            SetTimer();
        }
        public void Onreceivedata(ref byte[] buffer)
        {
#if UTF16
            var idstr = System.Text.Encoding.Unicode.GetString(buffer);
#else
            var idstr = System.Text.Encoding.UTF8.GetString(buffer);
#endif
            JObject jsonstr = JObject.Parse(idstr);
            try
            {
                int Command = (int)jsonstr.GetValue("Command");
                if (Command==1)
                {
                    float latitude = (float)jsonstr.GetValue("latitude");
                    float longitude = (float)jsonstr.GetValue("longitude");
                    int latitudei = (int)(latitude * 100);//km
                    int longitudei = (int)(longitude * 100);
                    position temp = new position(latitudei,longitudei);

                    Console.WriteLine("channelid:" + channelid + " latitude: "+ latitude+ " longitude: "+ longitude);
                    bool bcontain = virtualworld.Cells.ContainsKey(temp);
                    if (bcontain)
                    {

                    }
                    else
                    {
                        virtualworld.Cells[temp] = new HashSet<KcpChannel>();
                        if (currentposition.latitude / 1000 != temp.latitude || currentposition.longitude / 1000 != temp.longitude)
                        {
                            position p1 = new position(currentposition.latitude / 1000, currentposition.longitude / 1000);
                            bool bcontain1 = virtualworld.Cells.ContainsKey(p1);
                            if (bcontain1)
                            { 
                                virtualworld.Cells[p1]?.Remove(this);//remove from last cell
                            }
                        }
                    }
                    currentposition = new position((int)(latitude * 100000), (int)(longitude * 100000));

                    virtualworld.Cells[temp].Add(this);
                    fleshpotentialsyncell();
                    sentpeopleatrangetome();
                }

            }
            catch (Exception e)
            { 

            }

            timercounter=0;

        }
        
        public void send(ref byte[] buffer)
        {
            byte[] databuffer = new byte[idlength+buffer.Length];
#if UTF16
            UnicodeEncoding asen = new UnicodeEncoding();
#else
            ASCIIEncoding asen = new ASCIIEncoding();
#endif
            byte[] idbuffe = asen.GetBytes(channelid);

            Array.ConstrainedCopy(idbuffe, 0, databuffer, 0, idlength);
            Array.ConstrainedCopy(buffer, 0, databuffer, idlength, buffer.Length);

            mkcpclient.userlevelsend(ref databuffer);
        }
        public void send(String str)
        {
#if UTF16
            UnicodeEncoding asen = new UnicodeEncoding();
#else
            ASCIIEncoding asen = new ASCIIEncoding();
#endif
            byte[] strbuffe = asen.GetBytes(str);
            send(ref strbuffe);
        }
        private System.Timers.Timer aTimer;
        private int timercounter = 0;
        private void SetTimer()
        {
            // Create a timer with a two second interval.
            aTimer = new System.Timers.Timer(1000);
            // Hook up the Elapsed event for the timer. 
            aTimer.Elapsed += (Object source, ElapsedEventArgs e) => {
                if (timercounter++ > 10)
                {
                    position p1 = new position(currentposition.latitude / 1000, currentposition.longitude / 1000);
                    bool bcontain1 = virtualworld.Cells.ContainsKey(p1);
                    if (bcontain1)
                    {
                        virtualworld.Cells[p1]?.Remove(this);//remove from last cell
                    }
                }
            };
            aTimer.AutoReset = true;
            aTimer.Enabled = true;
        }
    }
}
