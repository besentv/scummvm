/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include <common/memstream.h>
#include "dragonimg.h"
#include "bigfile.h"

namespace Dragons {

DragonIMG::DragonIMG(BigfileArchive *bigfileArchive) {
	uint32 fileSize;
	byte *iptData = bigfileArchive->load("dragon.ipt", fileSize);
	Common::SeekableReadStream *iptReadStream = new Common::MemoryReadStream(iptData, fileSize, DisposeAfterUse::YES);

	_count = fileSize / 4;

	_imgData = bigfileArchive->load("dragon.img", fileSize);
	Common::SeekableReadStream *imgReadStream = new Common::MemoryReadStream(_imgData, fileSize, DisposeAfterUse::NO);


	_imgObjects = new IMG[_count];

	for(int i=0; i < _count; i++) {
		imgReadStream->seek(iptReadStream->readUint32LE());
		_imgObjects[i].field_0 = imgReadStream->readUint16LE();
		_imgObjects[i].field_2 = imgReadStream->readUint16LE();
		_imgObjects[i].field_4 = imgReadStream->readUint16LE();
		_imgObjects[i].field_6 = imgReadStream->readUint16LE();
		_imgObjects[i].field_8 = imgReadStream->readUint16LE();
		_imgObjects[i].field_a = imgReadStream->readUint16LE();
		_imgObjects[i].field_c = imgReadStream->readUint16LE();
		_imgObjects[i].field_e = imgReadStream->readUint16LE();
		_imgObjects[i].data = _imgData + imgReadStream->pos();
	}

	delete iptReadStream;
	delete imgReadStream;
}

DragonIMG::~DragonIMG() {
	delete _imgData;
}

IMG *DragonIMG::getIMG(uint32 iptId) {
	iptId &= 0xffff;
	assert(iptId > 1);
	assert(iptId < _count);
	return &_imgObjects[iptId];
}

} // End of namespace Dragons