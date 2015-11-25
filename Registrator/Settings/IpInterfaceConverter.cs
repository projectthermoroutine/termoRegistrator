using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

using System.Net.NetworkInformation;

namespace Registrator
{
   /// <summary>
   /// TypeConverter для списка IPv4 интерфейсов
   /// </summary>
    class IpInterfaceConverter : StringConverter
   {
        public IpInterfaceConverter():base()
        {
            _addresses = retrieve_ip4_interface_ips();
        }
      /// <summary>
      /// Будем предоставлять выбор из списка
      /// </summary>
      public override bool GetStandardValuesSupported(
         ITypeDescriptorContext context )
      {
         return true;
      }

      /// <summary>
      /// ... и только из списка
      /// </summary>
      public override bool GetStandardValuesExclusive(
         ITypeDescriptorContext context )
      {
         // false - можно вводить вручную
         // true - только выбор из списка
          return false;
      }

      /// <summary>
      /// А вот и список
      /// </summary>
      public override StandardValuesCollection GetStandardValues(
         ITypeDescriptorContext context )
      {
         // возвращаем список строк из настроек программы
         // (базы данных, интернет и т.д.)
          return new StandardValuesCollection(_addresses);
      }

      List<string> _addresses;

       List<string> retrieve_ip4_interface_ips()
      {
          List<string> result = new List<string>();

          NetworkInterface[] nics = NetworkInterface.GetAllNetworkInterfaces();

          foreach (NetworkInterface adapter in nics)
          {
              IPInterfaceProperties properties = adapter.GetIPProperties();

              if (adapter.Supports(NetworkInterfaceComponent.IPv4))
              {
                  foreach (UnicastIPAddressInformation uni in properties.UnicastAddresses)
                  {
                      if (uni.Address.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
                          result.Add(uni.Address.ToString());
                  }
              }
          }
          return result;

      }

   }
}
