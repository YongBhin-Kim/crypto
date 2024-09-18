<?php

class KISA_SEED
{
	static $KC0 = 0x9e3779b9;
	static $KC1 = 0x3c6ef373;
	static $KC2 = 0x78dde6e6;
	static $KC3 = 0xf1bbcdcc;
	static $KC4 = 0xe3779b99;
	static $KC5 = 0xc6ef3733;
	static $KC6 = 0x8dde6e67;
	static $KC7 = 0x1bbcdccf;
	static $KC8 = 0x3779b99e;
	static $KC9 = 0x6ef3733c;
	static $KC10 = 0xdde6e678;
	static $KC11 = 0xbbcdccf1;
	static $KC12 = 0x779b99e3;
	static $KC13 = 0xef3733c6;
	static $KC14 = 0xde6e678d;
    static $KC15 = 0xbcdccf1b;

	static $SS0 = array(
		0x02989a1a8, 0x005858184, 0x016c6d2d4, 0x013c3d3d0, 0x014445054, 0x01d0d111c, 0x02c8ca0ac, 0x025052124,
		0x01d4d515c, 0x003434340, 0x018081018, 0x01e0e121c, 0x011415150, 0x03cccf0fc, 0x00acac2c8, 0x023436360,
		0x028082028, 0x004444044, 0x020002020, 0x01d8d919c, 0x020c0e0e0, 0x022c2e2e0, 0x008c8c0c8, 0x017071314,
		0x02585a1a4, 0x00f8f838c, 0x003030300, 0x03b4b7378, 0x03b8bb3b8, 0x013031310, 0x012c2d2d0, 0x02ecee2ec,
		0x030407070, 0x00c8c808c, 0x03f0f333c, 0x02888a0a8, 0x032023230, 0x01dcdd1dc, 0x036c6f2f4, 0x034447074,
		0x02ccce0ec, 0x015859194, 0x00b0b0308, 0x017475354, 0x01c4c505c, 0x01b4b5358, 0x03d8db1bc, 0x001010100,
		0x024042024, 0x01c0c101c, 0x033437370, 0x018889098, 0x010001010, 0x00cccc0cc, 0x032c2f2f0, 0x019c9d1d8,
		0x02c0c202c, 0x027c7e3e4, 0x032427270, 0x003838380, 0x01b8b9398, 0x011c1d1d0, 0x006868284, 0x009c9c1c8,
		0x020406060, 0x010405050, 0x02383a3a0, 0x02bcbe3e8, 0x00d0d010c, 0x03686b2b4, 0x01e8e929c, 0x00f4f434c,
		0x03787b3b4, 0x01a4a5258, 0x006c6c2c4, 0x038487078, 0x02686a2a4, 0x012021210, 0x02f8fa3ac, 0x015c5d1d4,
		0x021416160, 0x003c3c3c0, 0x03484b0b4, 0x001414140, 0x012425250, 0x03d4d717c, 0x00d8d818c, 0x008080008,
		0x01f0f131c, 0x019899198, 0x000000000, 0x019091118, 0x004040004, 0x013435350, 0x037c7f3f4, 0x021c1e1e0,
		0x03dcdf1fc, 0x036467274, 0x02f0f232c, 0x027072324, 0x03080b0b0, 0x00b8b8388, 0x00e0e020c, 0x02b8ba3a8,
		0x02282a2a0, 0x02e4e626c, 0x013839390, 0x00d4d414c, 0x029496168, 0x03c4c707c, 0x009090108, 0x00a0a0208,
		0x03f8fb3bc, 0x02fcfe3ec, 0x033c3f3f0, 0x005c5c1c4, 0x007878384, 0x014041014, 0x03ecef2fc, 0x024446064,
		0x01eced2dc, 0x02e0e222c, 0x00b4b4348, 0x01a0a1218, 0x006060204, 0x021012120, 0x02b4b6368, 0x026466264,
		0x002020200, 0x035c5f1f4, 0x012829290, 0x00a8a8288, 0x00c0c000c, 0x03383b3b0, 0x03e4e727c, 0x010c0d0d0,
		0x03a4a7278, 0x007474344, 0x016869294, 0x025c5e1e4, 0x026062224, 0x000808080, 0x02d8da1ac, 0x01fcfd3dc,
		0x02181a1a0, 0x030003030, 0x037073334, 0x02e8ea2ac, 0x036063234, 0x015051114, 0x022022220, 0x038083038,
		0x034c4f0f4, 0x02787a3a4, 0x005454144, 0x00c4c404c, 0x001818180, 0x029c9e1e8, 0x004848084, 0x017879394,
		0x035053134, 0x00bcbc3c8, 0x00ecec2cc, 0x03c0c303c, 0x031417170, 0x011011110, 0x007c7c3c4, 0x009898188,
		0x035457174, 0x03bcbf3f8, 0x01acad2d8, 0x038c8f0f8, 0x014849094, 0x019495158, 0x002828280, 0x004c4c0c4,
		0x03fcff3fc, 0x009494148, 0x039093138, 0x027476364, 0x000c0c0c0, 0x00fcfc3cc, 0x017c7d3d4, 0x03888b0b8,
		0x00f0f030c, 0x00e8e828c, 0x002424240, 0x023032320, 0x011819190, 0x02c4c606c, 0x01bcbd3d8, 0x02484a0a4,
		0x034043034, 0x031c1f1f0, 0x008484048, 0x002c2c2c0, 0x02f4f636c, 0x03d0d313c, 0x02d0d212c, 0x000404040,
		0x03e8eb2bc, 0x03e0e323c, 0x03c8cb0bc, 0x001c1c1c0, 0x02a8aa2a8, 0x03a8ab2b8, 0x00e4e424c, 0x015455154,
		0x03b0b3338, 0x01cccd0dc, 0x028486068, 0x03f4f737c, 0x01c8c909c, 0x018c8d0d8, 0x00a4a4248, 0x016465254,
		0x037477374, 0x02080a0a0, 0x02dcde1ec, 0x006464244, 0x03585b1b4, 0x02b0b2328, 0x025456164, 0x03acaf2f8,
		0x023c3e3e0, 0x03989b1b8, 0x03181b1b0, 0x01f8f939c, 0x01e4e525c, 0x039c9f1f8, 0x026c6e2e4, 0x03282b2b0,
		0x031013130, 0x02acae2e8, 0x02d4d616c, 0x01f4f535c, 0x024c4e0e4, 0x030c0f0f0, 0x00dcdc1cc, 0x008888088,
		0x016061214, 0x03a0a3238, 0x018485058, 0x014c4d0d4, 0x022426260, 0x029092128, 0x007070304, 0x033033330,
		0x028c8e0e8, 0x01b0b1318, 0x005050104, 0x039497178, 0x010809090, 0x02a4a6268, 0x02a0a2228, 0x01a8a9298
	);

	static $SS1 = array(
		0x038380830, 0x0e828c8e0, 0x02c2d0d21, 0x0a42686a2, 0x0cc0fcfc3, 0x0dc1eced2, 0x0b03383b3, 0x0b83888b0,
		0x0ac2f8fa3, 0x060204060, 0x054154551, 0x0c407c7c3, 0x044044440, 0x06c2f4f63, 0x0682b4b63, 0x0581b4b53,
		0x0c003c3c3, 0x060224262, 0x030330333, 0x0b43585b1, 0x028290921, 0x0a02080a0, 0x0e022c2e2, 0x0a42787a3,
		0x0d013c3d3, 0x090118191, 0x010110111, 0x004060602, 0x01c1c0c10, 0x0bc3c8cb0, 0x034360632, 0x0480b4b43,
		0x0ec2fcfe3, 0x088088880, 0x06c2c4c60, 0x0a82888a0, 0x014170713, 0x0c404c4c0, 0x014160612, 0x0f434c4f0,
		0x0c002c2c2, 0x044054541, 0x0e021c1e1, 0x0d416c6d2, 0x03c3f0f33, 0x03c3d0d31, 0x08c0e8e82, 0x098188890,
		0x028280820, 0x04c0e4e42, 0x0f436c6f2, 0x03c3e0e32, 0x0a42585a1, 0x0f839c9f1, 0x00c0d0d01, 0x0dc1fcfd3,
		0x0d818c8d0, 0x0282b0b23, 0x064264662, 0x0783a4a72, 0x024270723, 0x02c2f0f23, 0x0f031c1f1, 0x070324272,
		0x040024242, 0x0d414c4d0, 0x040014141, 0x0c000c0c0, 0x070334373, 0x064274763, 0x0ac2c8ca0, 0x0880b8b83,
		0x0f437c7f3, 0x0ac2d8da1, 0x080008080, 0x01c1f0f13, 0x0c80acac2, 0x02c2c0c20, 0x0a82a8aa2, 0x034340430,
		0x0d012c2d2, 0x0080b0b03, 0x0ec2ecee2, 0x0e829c9e1, 0x05c1d4d51, 0x094148490, 0x018180810, 0x0f838c8f0,
		0x054174753, 0x0ac2e8ea2, 0x008080800, 0x0c405c5c1, 0x010130313, 0x0cc0dcdc1, 0x084068682, 0x0b83989b1,
		0x0fc3fcff3, 0x07c3d4d71, 0x0c001c1c1, 0x030310131, 0x0f435c5f1, 0x0880a8a82, 0x0682a4a62, 0x0b03181b1,
		0x0d011c1d1, 0x020200020, 0x0d417c7d3, 0x000020202, 0x020220222, 0x004040400, 0x068284860, 0x070314171,
		0x004070703, 0x0d81bcbd3, 0x09c1d8d91, 0x098198991, 0x060214161, 0x0bc3e8eb2, 0x0e426c6e2, 0x058194951,
		0x0dc1dcdd1, 0x050114151, 0x090108090, 0x0dc1cccd0, 0x0981a8a92, 0x0a02383a3, 0x0a82b8ba3, 0x0d010c0d0,
		0x080018181, 0x00c0f0f03, 0x044074743, 0x0181a0a12, 0x0e023c3e3, 0x0ec2ccce0, 0x08c0d8d81, 0x0bc3f8fb3,
		0x094168692, 0x0783b4b73, 0x05c1c4c50, 0x0a02282a2, 0x0a02181a1, 0x060234363, 0x020230323, 0x04c0d4d41,
		0x0c808c8c0, 0x09c1e8e92, 0x09c1c8c90, 0x0383a0a32, 0x00c0c0c00, 0x02c2e0e22, 0x0b83a8ab2, 0x06c2e4e62,
		0x09c1f8f93, 0x0581a4a52, 0x0f032c2f2, 0x090128292, 0x0f033c3f3, 0x048094941, 0x078384870, 0x0cc0cccc0,
		0x014150511, 0x0f83bcbf3, 0x070304070, 0x074354571, 0x07c3f4f73, 0x034350531, 0x010100010, 0x000030303,
		0x064244460, 0x06c2d4d61, 0x0c406c6c2, 0x074344470, 0x0d415c5d1, 0x0b43484b0, 0x0e82acae2, 0x008090901,
		0x074364672, 0x018190911, 0x0fc3ecef2, 0x040004040, 0x010120212, 0x0e020c0e0, 0x0bc3d8db1, 0x004050501,
		0x0f83acaf2, 0x000010101, 0x0f030c0f0, 0x0282a0a22, 0x05c1e4e52, 0x0a82989a1, 0x054164652, 0x040034343,
		0x084058581, 0x014140410, 0x088098981, 0x0981b8b93, 0x0b03080b0, 0x0e425c5e1, 0x048084840, 0x078394971,
		0x094178793, 0x0fc3cccf0, 0x01c1e0e12, 0x080028282, 0x020210121, 0x08c0c8c80, 0x0181b0b13, 0x05c1f4f53,
		0x074374773, 0x054144450, 0x0b03282b2, 0x01c1d0d11, 0x024250521, 0x04c0f4f43, 0x000000000, 0x044064642,
		0x0ec2dcde1, 0x058184850, 0x050124252, 0x0e82bcbe3, 0x07c3e4e72, 0x0d81acad2, 0x0c809c9c1, 0x0fc3dcdf1,
		0x030300030, 0x094158591, 0x064254561, 0x03c3c0c30, 0x0b43686b2, 0x0e424c4e0, 0x0b83b8bb3, 0x07c3c4c70,
		0x00c0e0e02, 0x050104050, 0x038390931, 0x024260622, 0x030320232, 0x084048480, 0x068294961, 0x090138393,
		0x034370733, 0x0e427c7e3, 0x024240420, 0x0a42484a0, 0x0c80bcbc3, 0x050134353, 0x0080a0a02, 0x084078783,
		0x0d819c9d1, 0x04c0c4c40, 0x080038383, 0x08c0f8f83, 0x0cc0ecec2, 0x0383b0b33, 0x0480a4a42, 0x0b43787b3
	);

	static $SS2 = array(
		0x0a1a82989, 0x081840585, 0x0d2d416c6, 0x0d3d013c3, 0x050541444, 0x0111c1d0d, 0x0a0ac2c8c, 0x021242505,
		0x0515c1d4d, 0x043400343, 0x010181808, 0x0121c1e0e, 0x051501141, 0x0f0fc3ccc, 0x0c2c80aca, 0x063602343,
		0x020282808, 0x040440444, 0x020202000, 0x0919c1d8d, 0x0e0e020c0, 0x0e2e022c2, 0x0c0c808c8, 0x013141707,
		0x0a1a42585, 0x0838c0f8f, 0x003000303, 0x073783b4b, 0x0b3b83b8b, 0x013101303, 0x0d2d012c2, 0x0e2ec2ece,
		0x070703040, 0x0808c0c8c, 0x0333c3f0f, 0x0a0a82888, 0x032303202, 0x0d1dc1dcd, 0x0f2f436c6, 0x070743444,
		0x0e0ec2ccc, 0x091941585, 0x003080b0b, 0x053541747, 0x0505c1c4c, 0x053581b4b, 0x0b1bc3d8d, 0x001000101,
		0x020242404, 0x0101c1c0c, 0x073703343, 0x090981888, 0x010101000, 0x0c0cc0ccc, 0x0f2f032c2, 0x0d1d819c9,
		0x0202c2c0c, 0x0e3e427c7, 0x072703242, 0x083800383, 0x093981b8b, 0x0d1d011c1, 0x082840686, 0x0c1c809c9,
		0x060602040, 0x050501040, 0x0a3a02383, 0x0e3e82bcb, 0x0010c0d0d, 0x0b2b43686, 0x0929c1e8e, 0x0434c0f4f,
		0x0b3b43787, 0x052581a4a, 0x0c2c406c6, 0x070783848, 0x0a2a42686, 0x012101202, 0x0a3ac2f8f, 0x0d1d415c5,
		0x061602141, 0x0c3c003c3, 0x0b0b43484, 0x041400141, 0x052501242, 0x0717c3d4d, 0x0818c0d8d, 0x000080808,
		0x0131c1f0f, 0x091981989, 0x000000000, 0x011181909, 0x000040404, 0x053501343, 0x0f3f437c7, 0x0e1e021c1,
		0x0f1fc3dcd, 0x072743646, 0x0232c2f0f, 0x023242707, 0x0b0b03080, 0x083880b8b, 0x0020c0e0e, 0x0a3a82b8b,
		0x0a2a02282, 0x0626c2e4e, 0x093901383, 0x0414c0d4d, 0x061682949, 0x0707c3c4c, 0x001080909, 0x002080a0a,
		0x0b3bc3f8f, 0x0e3ec2fcf, 0x0f3f033c3, 0x0c1c405c5, 0x083840787, 0x010141404, 0x0f2fc3ece, 0x060642444,
		0x0d2dc1ece, 0x0222c2e0e, 0x043480b4b, 0x012181a0a, 0x002040606, 0x021202101, 0x063682b4b, 0x062642646,
		0x002000202, 0x0f1f435c5, 0x092901282, 0x082880a8a, 0x0000c0c0c, 0x0b3b03383, 0x0727c3e4e, 0x0d0d010c0,
		0x072783a4a, 0x043440747, 0x092941686, 0x0e1e425c5, 0x022242606, 0x080800080, 0x0a1ac2d8d, 0x0d3dc1fcf,
		0x0a1a02181, 0x030303000, 0x033343707, 0x0a2ac2e8e, 0x032343606, 0x011141505, 0x022202202, 0x030383808,
		0x0f0f434c4, 0x0a3a42787, 0x041440545, 0x0404c0c4c, 0x081800181, 0x0e1e829c9, 0x080840484, 0x093941787,
		0x031343505, 0x0c3c80bcb, 0x0c2cc0ece, 0x0303c3c0c, 0x071703141, 0x011101101, 0x0c3c407c7, 0x081880989,
		0x071743545, 0x0f3f83bcb, 0x0d2d81aca, 0x0f0f838c8, 0x090941484, 0x051581949, 0x082800282, 0x0c0c404c4,
		0x0f3fc3fcf, 0x041480949, 0x031383909, 0x063642747, 0x0c0c000c0, 0x0c3cc0fcf, 0x0d3d417c7, 0x0b0b83888,
		0x0030c0f0f, 0x0828c0e8e, 0x042400242, 0x023202303, 0x091901181, 0x0606c2c4c, 0x0d3d81bcb, 0x0a0a42484,
		0x030343404, 0x0f1f031c1, 0x040480848, 0x0c2c002c2, 0x0636c2f4f, 0x0313c3d0d, 0x0212c2d0d, 0x040400040,
		0x0b2bc3e8e, 0x0323c3e0e, 0x0b0bc3c8c, 0x0c1c001c1, 0x0a2a82a8a, 0x0b2b83a8a, 0x0424c0e4e, 0x051541545,
		0x033383b0b, 0x0d0dc1ccc, 0x060682848, 0x0737c3f4f, 0x0909c1c8c, 0x0d0d818c8, 0x042480a4a, 0x052541646,
		0x073743747, 0x0a0a02080, 0x0e1ec2dcd, 0x042440646, 0x0b1b43585, 0x023282b0b, 0x061642545, 0x0f2f83aca,
		0x0e3e023c3, 0x0b1b83989, 0x0b1b03181, 0x0939c1f8f, 0x0525c1e4e, 0x0f1f839c9, 0x0e2e426c6, 0x0b2b03282,
		0x031303101, 0x0e2e82aca, 0x0616c2d4d, 0x0535c1f4f, 0x0e0e424c4, 0x0f0f030c0, 0x0c1cc0dcd, 0x080880888,
		0x012141606, 0x032383a0a, 0x050581848, 0x0d0d414c4, 0x062602242, 0x021282909, 0x003040707, 0x033303303,
		0x0e0e828c8, 0x013181b0b, 0x001040505, 0x071783949, 0x090901080, 0x062682a4a, 0x022282a0a, 0x092981a8a
	);

	static $SS3 = array(
		0x008303838, 0x0c8e0e828, 0x00d212c2d, 0x086a2a426, 0x0cfc3cc0f, 0x0ced2dc1e, 0x083b3b033, 0x088b0b838,
		0x08fa3ac2f, 0x040606020, 0x045515415, 0x0c7c3c407, 0x044404404, 0x04f636c2f, 0x04b63682b, 0x04b53581b,
		0x0c3c3c003, 0x042626022, 0x003333033, 0x085b1b435, 0x009212829, 0x080a0a020, 0x0c2e2e022, 0x087a3a427,
		0x0c3d3d013, 0x081919011, 0x001111011, 0x006020406, 0x00c101c1c, 0x08cb0bc3c, 0x006323436, 0x04b43480b,
		0x0cfe3ec2f, 0x088808808, 0x04c606c2c, 0x088a0a828, 0x007131417, 0x0c4c0c404, 0x006121416, 0x0c4f0f434,
		0x0c2c2c002, 0x045414405, 0x0c1e1e021, 0x0c6d2d416, 0x00f333c3f, 0x00d313c3d, 0x08e828c0e, 0x088909818,
		0x008202828, 0x04e424c0e, 0x0c6f2f436, 0x00e323c3e, 0x085a1a425, 0x0c9f1f839, 0x00d010c0d, 0x0cfd3dc1f,
		0x0c8d0d818, 0x00b23282b, 0x046626426, 0x04a72783a, 0x007232427, 0x00f232c2f, 0x0c1f1f031, 0x042727032,
		0x042424002, 0x0c4d0d414, 0x041414001, 0x0c0c0c000, 0x043737033, 0x047636427, 0x08ca0ac2c, 0x08b83880b,
		0x0c7f3f437, 0x08da1ac2d, 0x080808000, 0x00f131c1f, 0x0cac2c80a, 0x00c202c2c, 0x08aa2a82a, 0x004303434,
		0x0c2d2d012, 0x00b03080b, 0x0cee2ec2e, 0x0c9e1e829, 0x04d515c1d, 0x084909414, 0x008101818, 0x0c8f0f838,
		0x047535417, 0x08ea2ac2e, 0x008000808, 0x0c5c1c405, 0x003131013, 0x0cdc1cc0d, 0x086828406, 0x089b1b839,
		0x0cff3fc3f, 0x04d717c3d, 0x0c1c1c001, 0x001313031, 0x0c5f1f435, 0x08a82880a, 0x04a62682a, 0x081b1b031,
		0x0c1d1d011, 0x000202020, 0x0c7d3d417, 0x002020002, 0x002222022, 0x004000404, 0x048606828, 0x041717031,
		0x007030407, 0x0cbd3d81b, 0x08d919c1d, 0x089919819, 0x041616021, 0x08eb2bc3e, 0x0c6e2e426, 0x049515819,
		0x0cdd1dc1d, 0x041515011, 0x080909010, 0x0ccd0dc1c, 0x08a92981a, 0x083a3a023, 0x08ba3a82b, 0x0c0d0d010,
		0x081818001, 0x00f030c0f, 0x047434407, 0x00a12181a, 0x0c3e3e023, 0x0cce0ec2c, 0x08d818c0d, 0x08fb3bc3f,
		0x086929416, 0x04b73783b, 0x04c505c1c, 0x082a2a022, 0x081a1a021, 0x043636023, 0x003232023, 0x04d414c0d,
		0x0c8c0c808, 0x08e929c1e, 0x08c909c1c, 0x00a32383a, 0x00c000c0c, 0x00e222c2e, 0x08ab2b83a, 0x04e626c2e,
		0x08f939c1f, 0x04a52581a, 0x0c2f2f032, 0x082929012, 0x0c3f3f033, 0x049414809, 0x048707838, 0x0ccc0cc0c,
		0x005111415, 0x0cbf3f83b, 0x040707030, 0x045717435, 0x04f737c3f, 0x005313435, 0x000101010, 0x003030003,
		0x044606424, 0x04d616c2d, 0x0c6c2c406, 0x044707434, 0x0c5d1d415, 0x084b0b434, 0x0cae2e82a, 0x009010809,
		0x046727436, 0x009111819, 0x0cef2fc3e, 0x040404000, 0x002121012, 0x0c0e0e020, 0x08db1bc3d, 0x005010405,
		0x0caf2f83a, 0x001010001, 0x0c0f0f030, 0x00a22282a, 0x04e525c1e, 0x089a1a829, 0x046525416, 0x043434003,
		0x085818405, 0x004101414, 0x089818809, 0x08b93981b, 0x080b0b030, 0x0c5e1e425, 0x048404808, 0x049717839,
		0x087939417, 0x0ccf0fc3c, 0x00e121c1e, 0x082828002, 0x001212021, 0x08c808c0c, 0x00b13181b, 0x04f535c1f,
		0x047737437, 0x044505414, 0x082b2b032, 0x00d111c1d, 0x005212425, 0x04f434c0f, 0x000000000, 0x046424406,
		0x0cde1ec2d, 0x048505818, 0x042525012, 0x0cbe3e82b, 0x04e727c3e, 0x0cad2d81a, 0x0c9c1c809, 0x0cdf1fc3d,
		0x000303030, 0x085919415, 0x045616425, 0x00c303c3c, 0x086b2b436, 0x0c4e0e424, 0x08bb3b83b, 0x04c707c3c,
		0x00e020c0e, 0x040505010, 0x009313839, 0x006222426, 0x002323032, 0x084808404, 0x049616829, 0x083939013,
		0x007333437, 0x0c7e3e427, 0x004202424, 0x084a0a424, 0x0cbc3c80b, 0x043535013, 0x00a02080a, 0x087838407,
		0x0c9d1d819, 0x04c404c0c, 0x083838003, 0x08f838c0f, 0x0cec2cc0e, 0x00b33383b, 0x04a42480a, 0x087b3b437
	);

    static function GetB0($A){ return 0x000000ff & $A; }
    static function GetB1($A){ return 0x000000ff & ( $A >> 8 ); }
    static function GetB2($A){ return 0x000000ff & ( $A >> 16 ); }
	static function GetB3($A){ return 0x000000ff & ( $A >> 24 ); }
	
    static function RoundKeyUpdate0(&$T, &$K, $K_offset, &$ABCD, $KC)
    {
		$T[0] = (($ABCD[0]&0x0ffffffff) + ($ABCD[2]&0x0ffffffff) - ($KC&0x0ffffffff)) & 0x0ffffffff;
		$T[1] = (($ABCD[1]&0x0ffffffff) + ($KC&0x0ffffffff) - ($ABCD[3]&0x0ffffffff)) & 0x0ffffffff;
        $K[$K_offset+0] = KISA_SEED::$SS0[KISA_SEED::GetB0($T[0])&0x0ff] ^ KISA_SEED::$SS1[KISA_SEED::GetB1($T[0])&0x0ff] ^
                          KISA_SEED::$SS2[KISA_SEED::GetB2($T[0])&0x0ff] ^ KISA_SEED::$SS3[KISA_SEED::GetB3($T[0])&0x0ff];
        $K[$K_offset+1] = KISA_SEED::$SS0[KISA_SEED::GetB0($T[1])&0x0ff] ^ KISA_SEED::$SS1[KISA_SEED::GetB1($T[1])&0x0ff] ^
                          KISA_SEED::$SS2[KISA_SEED::GetB2($T[1])&0x0ff] ^ KISA_SEED::$SS3[KISA_SEED::GetB3($T[1])&0x0ff];
		$T[0] = $ABCD[0]; 
		$ABCD[0] = (($ABCD[0]>>8)&0x00ffffff) ^ ($ABCD[1]<<24);
		$ABCD[1] = (($ABCD[1]>>8)&0x00ffffff) ^ ($T[0]<<24);
	}

    static function RoundKeyUpdate1(&$T, &$K, $K_offset, &$ABCD, $KC)
    {
		$T[0] = (($ABCD[0]&0xFFFFFFFF) + ($ABCD[2]&0xFFFFFFFF) - ($KC&0xFFFFFFFF)) & 0x0ffffffff;
		$T[1] = (($ABCD[1]&0xFFFFFFFF) + ($KC&0xFFFFFFFF) - ($ABCD[3]&0xFFFFFFFF)) & 0x0ffffffff;
        $K[$K_offset+0] = KISA_SEED::$SS0[KISA_SEED::GetB0($T[0])&0x0ff] ^ KISA_SEED::$SS1[KISA_SEED::GetB1($T[0])&0x0ff] ^
                          KISA_SEED::$SS2[KISA_SEED::GetB2($T[0])&0x0ff] ^ KISA_SEED::$SS3[KISA_SEED::GetB3($T[0])&0x0ff];
        $K[$K_offset+1] = KISA_SEED::$SS0[KISA_SEED::GetB0($T[1])&0x0ff] ^ KISA_SEED::$SS1[KISA_SEED::GetB1($T[1])&0x0ff] ^
                          KISA_SEED::$SS2[KISA_SEED::GetB2($T[1])&0x0ff] ^ KISA_SEED::$SS3[KISA_SEED::GetB3($T[1])&0x0ff];
		$T[0] = $ABCD[2];
		$ABCD[2] = ($ABCD[2]<<8) ^ (($ABCD[3]>>24)&0x000000ff);
		$ABCD[3] = ($ABCD[3]<<8) ^ (($T[0]>>24)&0x000000ff);
	}

    static function SeedRound(&$T, &$LR, $L0, $L1, $R0, $R1, &$K, $K_offset)
    {
		$T[0] = $LR[$R0] ^ $K[$K_offset+0];
		$T[1] = $LR[$R1] ^ $K[$K_offset+1];
		$T[1] ^= $T[0];
		$T[1] = KISA_SEED::$SS0[KISA_SEED::GetB0($T[1])&0x0ff] ^ KISA_SEED::$SS1[KISA_SEED::GetB1($T[1])&0x0ff] ^
				KISA_SEED::$SS2[KISA_SEED::GetB2($T[1])&0x0ff] ^ KISA_SEED::$SS3[KISA_SEED::GetB3($T[1])&0x0ff];
		$T[0] = ($T[0] + $T[1]) & 0x0ffffffff;
		$T[0] = KISA_SEED::$SS0[KISA_SEED::GetB0($T[0])&0x0ff] ^ KISA_SEED::$SS1[KISA_SEED::GetB1($T[0])&0x0ff] ^
				KISA_SEED::$SS2[KISA_SEED::GetB2($T[0])&0x0ff] ^ KISA_SEED::$SS3[KISA_SEED::GetB3($T[0])&0x0ff];
		$T[1] = ($T[1] + $T[0]) & 0x0ffffffff;
		$T[1] = KISA_SEED::$SS0[KISA_SEED::GetB0($T[1])&0x0ff] ^ KISA_SEED::$SS1[KISA_SEED::GetB1($T[1])&0x0ff] ^
				KISA_SEED::$SS2[KISA_SEED::GetB2($T[1])&0x0ff] ^ KISA_SEED::$SS3[KISA_SEED::GetB3($T[1])&0x0ff];
		$T[0] = ($T[0] + $T[1]) & 0x0ffffffff;
		$LR[$L0] ^= $T[0]; $LR[$L1] ^= $T[1];
    }
    
    static function Byte2Word($src, $src_offset)
    {
        return (($src[$src_offset] & 0x0FF) << 24) | (($src[$src_offset + 1] & 0x0FF) << 16) | (($src[$src_offset + 2] & 0x0FF) << 8) | (($src[$src_offset + 3] & 0x0FF));
    }

	static function SEED_KeySched($mKey, &$rKey)
	{
		$ABCD = array_pad(array(),4,0);
		$T = array_pad(array(),2,0);

		$ABCD[0] = KISA_SEED::Byte2Word($mKey, 0);
        $ABCD[1] = KISA_SEED::Byte2Word($mKey, 4);
        $ABCD[2] = KISA_SEED::Byte2Word($mKey, 8);
        $ABCD[3] = KISA_SEED::Byte2Word($mKey,12);

        KISA_SEED::RoundKeyUpdate0($T, $rKey, 0, $ABCD, KISA_SEED::$KC0 );
        KISA_SEED::RoundKeyUpdate1($T, $rKey, 2, $ABCD, KISA_SEED::$KC1 );
        KISA_SEED::RoundKeyUpdate0($T, $rKey, 4, $ABCD, KISA_SEED::$KC2 );
        KISA_SEED::RoundKeyUpdate1($T, $rKey, 6, $ABCD, KISA_SEED::$KC3 );
        KISA_SEED::RoundKeyUpdate0($T, $rKey, 8, $ABCD, KISA_SEED::$KC4 );
        KISA_SEED::RoundKeyUpdate1($T, $rKey, 10, $ABCD, KISA_SEED::$KC5 );
        KISA_SEED::RoundKeyUpdate0($T, $rKey, 12, $ABCD, KISA_SEED::$KC6 );
        KISA_SEED::RoundKeyUpdate1($T, $rKey, 14, $ABCD, KISA_SEED::$KC7 );
        KISA_SEED::RoundKeyUpdate0($T, $rKey, 16, $ABCD, KISA_SEED::$KC8 );
        KISA_SEED::RoundKeyUpdate1($T, $rKey, 18, $ABCD, KISA_SEED::$KC9 );
        KISA_SEED::RoundKeyUpdate0($T, $rKey, 20, $ABCD, KISA_SEED::$KC10);
        KISA_SEED::RoundKeyUpdate1($T, $rKey, 22, $ABCD, KISA_SEED::$KC11);
        KISA_SEED::RoundKeyUpdate0($T, $rKey, 24, $ABCD, KISA_SEED::$KC12);
        KISA_SEED::RoundKeyUpdate1($T, $rKey, 26, $ABCD, KISA_SEED::$KC13);
        KISA_SEED::RoundKeyUpdate0($T, $rKey, 28, $ABCD, KISA_SEED::$KC14);

        $T[0] = (($ABCD[0]&0xFFFFFFFF) + ($ABCD[2]&0xFFFFFFFF) - (KISA_SEED::$KC15&0xFFFFFFFF))&0xFFFFFFFF;
        $T[1] = (($ABCD[1]&0xFFFFFFFF) - ($ABCD[3]&0xFFFFFFFF) + (KISA_SEED::$KC15&0xFFFFFFFF))&0xFFFFFFFF;
        
        $rKey[30] = KISA_SEED::$SS0[KISA_SEED::GetB0($T[0]) & 0x0FF] ^ KISA_SEED::$SS1[KISA_SEED::GetB1($T[0]) & 0x0FF] ^
                    KISA_SEED::$SS2[KISA_SEED::GetB2($T[0]) & 0x0FF] ^ KISA_SEED::$SS3[KISA_SEED::GetB3($T[0]) & 0x0FF];
        $rKey[31] = KISA_SEED::$SS0[KISA_SEED::GetB0($T[1]) & 0x0FF] ^ KISA_SEED::$SS1[KISA_SEED::GetB1($T[1]) & 0x0FF] ^
                    KISA_SEED::$SS2[KISA_SEED::GetB2($T[1]) & 0x0FF] ^ KISA_SEED::$SS3[KISA_SEED::GetB3($T[1]) & 0x0FF];
	}

	static function SEED_Encrypt(&$pOut, $pIn, $rKey)
	{
		$LR = array_pad(array(), 4, 0);
		$T = array_pad(array(), 2, 0);

		$LR[0] = $pIn[0];
		$LR[1] = $pIn[1];
		$LR[2] = $pIn[2];
		$LR[3] = $pIn[3];

		KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey,  0);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey,  2);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey,  4);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey,  6);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey,  8);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey, 10);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey, 12);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey, 14);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey, 16);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey, 18);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey, 20);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey, 22);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey, 24);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey, 26);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey, 28);
		KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey, 30);
		
        $pOut[0] = $LR[2];
        $pOut[1] = $LR[3];
        $pOut[2] = $LR[0];
        $pOut[3] = $LR[1];
	}

	static function SEED_Decrypt(&$pOut, $pIn, $rKey)
	{
		$LR = array_pad(array(), 4, 0);
		$T = array_pad(array(), 2, 0);

		$LR[0] = $pIn[0];
		$LR[1] = $pIn[1];
		$LR[2] = $pIn[2];
		$LR[3] = $pIn[3];

		KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey, 30);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey, 28);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey, 26);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey, 24);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey, 22);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey, 20);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey, 18);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey, 16);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey, 14);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey, 12);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey, 10);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey,  8);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey,  6);
        KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey,  4);
        KISA_SEED::SeedRound($T, $LR, 0, 1, 2, 3, $rKey,  2);
		KISA_SEED::SeedRound($T, $LR, 2, 3, 0, 1, $rKey,  0);
		
        $pOut[0] = $LR[2];
        $pOut[1] = $LR[3];
        $pOut[2] = $LR[0];
        $pOut[3] = $LR[1];
	}
}
?>