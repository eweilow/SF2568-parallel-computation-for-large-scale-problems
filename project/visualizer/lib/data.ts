export type Rabbit = {
  birthDay: number;
  id: number;
};

export type Fox = {
  birthDay: number;
  id: number;
  hunger: number;
};

export type TileData = {
  vegetation: number;
  rabbitsCount: number;
  rabbits: Rabbit[];
  foxesCount: number;
  foxes: Fox[];
};
export type Tile = {
  x: number;
  y: number;
  id: number;
  process: number;
  isOwnedByThisProcess: boolean;
  isWaterTile: boolean;
  historicalDataCount: number;
  historicalDatas: TileData[];
};

export type Data = {
  rabbitSize: number;
  foxSize: number;
  tileCount: number;
  tileSize: number;
  tiles: Tile[];
};

export function processData(buffer: ArrayBuffer) {
  const ds = new DataView(buffer);
  let offset = 0;
  const nextInt64 = () => {
    const val = ds.getBigInt64(offset, true);
    offset += 8;
    return Number(val);
  };
  const nextUInt64 = () => {
    const val = ds.getBigUint64(offset, true);
    offset += 8;
    return Number(val);
  };
  const nextDouble = () => {
    const val = ds.getFloat64(offset, true);
    offset += 8;
    return val;
  };
  const nextByte = () => {
    const val = ds.getInt8(offset);
    offset += 1;
    return val;
  };

  const nextBool = () => {
    return nextByte() === 1;
  };

  const tileCount = nextInt64();
  const tileSize = nextDouble();

  const rabbitSize = nextInt64();
  const foxSize = nextInt64();

  const data = {
    rabbitSize,
    foxSize,
    tileCount,
    tileSize,
    tiles: [],
  } as Data;

  for (let i = 0; i < tileCount; i++) {
    const x = nextDouble();
    const y = nextDouble();
    const id = nextUInt64();
    const process = nextInt64();

    const isOwnedByThisProcess = nextBool();
    const isWaterTile = nextBool();

    const historicalDataCount = nextInt64();

    const tile = {
      x,
      y,
      id,
      process,
      isOwnedByThisProcess,
      isWaterTile,
      historicalDataCount,
      historicalDatas: [],
    } as Tile;

    for (let j = 0; j < historicalDataCount; j++) {
      const tileData = {
        vegetation: nextDouble(),
        rabbitsCount: 0,
        rabbits: [],
        foxesCount: 0,
        foxes: [],
      } as TileData;

      tileData.rabbitsCount = nextInt64();
      let baseRabbitsOffset = offset;
      for (let k = 0; k < tileData.rabbitsCount; k++) {
        offset = baseRabbitsOffset + k * rabbitSize;

        const rabbit = {
          birthDay: nextInt64(),
          id: nextUInt64(),
        } as Rabbit;
        tileData.rabbits.push(rabbit);
      }
      offset = baseRabbitsOffset + tileData.rabbitsCount * rabbitSize;
      tileData.foxesCount = nextInt64();

      let baseFoxesOffset = offset;
      for (let k = 0; k < tileData.foxesCount; k++) {
        offset = baseFoxesOffset + k * foxSize;

        const fox = {
          birthDay: nextInt64(),
          id: nextUInt64(),
          hunger: nextDouble(),
        } as Fox;
        tileData.foxes.push(fox);
      }
      offset = baseFoxesOffset + tileData.foxesCount * foxSize;

      tile.historicalDatas.push(tileData);
    }
    data.tiles.push(tile);
  }
  return data;
}
