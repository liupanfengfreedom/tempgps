﻿#define LOOPBACKTEST
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Net.Sockets.Kcp;
using System.Buffers;
using System.Threading.Tasks;

namespace ChatServer
{
    public delegate void OnUserLevelReceivedCompleted(ref byte[] buffer);
    class KcpClient : UdpClient
    {
        Kcp mKcp;
        public OnUserLevelReceivedCompleted onUserLevelReceivedCompleted;
        public KcpClient(UdpServer udpserver, EndPoint remoteendpoint) : base(udpserver, remoteendpoint)
        {
#if LOOPBACKTEST
            onUserLevelReceivedCompleted += (ref byte[] buffer) => {
                byte[] idbuffer = new byte[KcpChannel.idlength];
                Array.ConstrainedCopy(buffer,0, idbuffer, 0, 32);
#if UTF16
                var idstr = System.Text.Encoding.Unicode.GetString(idbuffer);
#else
                var idstr = System.Text.Encoding.UTF8.GetString(idbuffer);
#endif
                bool bcontain = KcpChannelManager.OnchannelReceivedatacallbackmap.ContainsKey(idstr);
                if (bcontain)
                {
                    if (KcpChannelManager.OnchannelReceivedatacallbackmap[idstr].mkcpclient.mremoteEP == mremoteEP)
                    {

                    }
                    else
                    {
                       // KcpChannelManager.OnchannelReceivedatacallbackmap[idstr].mkcpclient.mremoteEP = mremoteEP;

                        KcpChannelManager.OnchannelReceivedatacallbackmap.Remove(idstr);
                        KcpChannelManager.OnchannelReceivedatacallbackmap.Add(idstr, new KcpChannel(idstr, this));//need verify
                    }
                }
                else
                { 
                    KcpChannelManager.OnchannelReceivedatacallbackmap.Add(idstr, new KcpChannel(idstr, this));
                }
                int datasize = buffer.Length - KcpChannel.idlength;
                byte[] databuffer = new byte[datasize];
                Array.ConstrainedCopy(buffer, KcpChannel.idlength, databuffer, 0, datasize);
                KcpChannelManager.OnchannelReceivedatacallbackmap[idstr].Onreceivedata(ref databuffer);

                #region 
                //byte[] latitudebuffer = new byte[4];
                //Array.ConstrainedCopy(buffer, 32, latitudebuffer, 0, 4);
                //float latitude = ByteArraytoChannelidType.DeSerialize(ref latitudebuffer);

                //byte[] longitudebuffer = new byte[4];
                //Array.ConstrainedCopy(buffer, 32+4, longitudebuffer, 0, 4);
                //float longitude = ByteArraytoChannelidType.DeSerialize(ref longitudebuffer);

                //Console.ForegroundColor = ConsoleColor.Blue;
                //Console.WriteLine(idstr);
                //Console.WriteLine(latitude.ToString());
                //Console.WriteLine(longitude.ToString());
                //Console.ForegroundColor = ConsoleColor.White;
                //userlevelsend(ref buffer);
                #endregion 
            };
#endif
            kcphandle handle = new kcphandle();
            OnReceivedCompletePointer = (ref byte[] buffer) => { 
                mKcp.Input(buffer);//buffer is received from low level data
            };
            handle.Out = buffer => {
                mudpserver.UdpListener.SendTo(buffer.ToArray(), mremoteEP);//low level send
            };
            mKcp = new Kcp((uint)(5598781), handle);
            mKcp.NoDelay(1, 5, 2, 1);//fast
            mKcp.WndSize(64, 64);
            mKcp.SetMtu(512);
            Task.Run(async () =>
            {
                try
                {
                    int updateCount = 0;
                    while (true)
                    {
                        mKcp.Update(DateTime.UtcNow);

                        int len;
                        while ((len = mKcp.PeekSize()) > 0)
                        {
                            var buffer = new byte[len];
                            if (mKcp.Recv(buffer) >= 0)
                            {
                                onUserLevelReceivedCompleted.Invoke(ref buffer);
                            }
                        }
                        await Task.Delay(1);
                        updateCount++;
                        if (updateCount % 1000 == 0)
                        {
                            Console.WriteLine($"KCP ALIVE {updateCount}----");
                        }
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                }

            });
        }
        public void userlevelsend(ref byte[] buffer)
        {
            var res = mKcp.Send(buffer);//send by handle.out
            if (res != 0)
            {
                Console.WriteLine($"kcp send error");
            }
        }

    }
    internal class kcphandle : IKcpCallback
    {
        public Action<Memory<byte>> Out;
        public void Output(IMemoryOwner<byte> buffer, int avalidLength)
        {
            Out(buffer.Memory.Slice(0, avalidLength));
        }
    }
    static class ByteArraytoChannelidType
    {
        public static float DeSerialize(ref byte[] m)
        {
            int sizeofidtype = sizeof(float);
            int sizeofarray = m.Length;
            int validsize = sizeofidtype < sizeofarray ? sizeofidtype : sizeofarray;
            float channelid;
            unsafe
            {
                byte* channelidp = (byte*)&channelid;
                for (int i = 0; i < validsize; i++)
                {
                    *(channelidp + i) = m[i];
                }
            }
            return channelid;
        }
        public static byte[] Serialize(float m)
        {
            int sizeofidtype = sizeof(float);
            byte[] bytearray = new byte[sizeofidtype];
            unsafe
            {
                byte* channelidp = (byte*)&m;
                for (int i = 0; i < sizeofidtype; i++)
                {
                    bytearray[i] = *(channelidp + i);
                }
            }
            return bytearray;
        }
    }
}
