using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ChatServer
{
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
        KcpClient mkcpclient;
        public KcpChannel(String channelidp, KcpClient kcpclientp )
        {
            channelid = channelidp;
            mkcpclient = kcpclientp;
        }
        public void Onreceivedata(ref byte[] buffer)
        {
#if UTF16
            var idstr = System.Text.Encoding.Unicode.GetString(buffer);
#else
            var idstr = System.Text.Encoding.UTF8.GetString(buffer);
#endif
            send(ref buffer);
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
            //mkcpclient.userlevelsend(ref buffer);
        }
    }
}
