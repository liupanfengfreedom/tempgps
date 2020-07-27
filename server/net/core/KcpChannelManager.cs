using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
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
            JObject jObject = new JObject();
            jObject.Add("Command", 1);
            jObject.Add("channelid", "1122233445566");
            jObject.Add("latitude", 30.5531);
            jObject.Add("longitude", 119.2634);
            send(jObject.ToString());
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
    }
}
