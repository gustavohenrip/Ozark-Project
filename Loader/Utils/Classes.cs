using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Media.Imaging;

namespace Loader {
    public class Classes {
		public enum eCheats : int {
			CHEAT_GRAND_THEFT_AUTO_V = 1,
			CHEAT_RED_DEAD_REDEMPTION_2,
			CHEAT_SEA_OF_THIEVES,
			CHEAT_PHASMOPHOBIA,
			CHEAT_BATTLEFIELD_3,
			CHEAT_BATTLEFIELD_4,
			CHEAT_BATTLEFIELD_V,
			CHEAT_END
		};

		public class Cheat {
            public string Name;
            public int Version;
            public int SecondsLeft;
            public eCheats CheatID;
            public bool Online;
            public string Process;
            public int InjectionTimeout;

            // UI
            public Border BorderHandle;
            public int Index;
            public bool GameOnline;
            public bool GameOnlineQueried;
            public BitmapImage ThumbnailImage;
            public BitmapImage Image;
        };

        public class Resource {
            public string Name;
            public string Author;
            public string Folder;
            public int Size;

            // UI
            public Border BorderHandle;
            public int Index;
        };
    }
}
