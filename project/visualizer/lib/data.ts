export interface AnimalLocation {
  tile: number;
}

export interface Animal<T> {
  birthDay: number;
  id: number;
  historicalData: Map<number, T>;
}

export interface Rabbit extends Animal<AnimalLocation> {}
export interface Fox extends Animal<AnimalLocation & { hunger: number }> {}

export type TileData = {
  vegetation: number;
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
  tileCount: number;
  tileSize: number;
  rabbits: Map<number, Rabbit>;
  foxes: Map<number, Fox>;
  tiles: Map<number, Tile>;
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
    tileCount,
    tileSize,
    tiles: new Map(),
    foxes: new Map(),
    rabbits: new Map(),
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

      const rabbitsCount = nextInt64();
      let baseRabbitsOffset = offset;
      for (let k = 0; k < rabbitsCount; k++) {
        offset = baseRabbitsOffset + k * rabbitSize;

        const rabbit = {
          birthDay: nextInt64(),
          id: nextUInt64(),
          historicalData: new Map(),
        } as Rabbit;

        if (!data.rabbits.has(rabbit.id)) {
          data.rabbits.set(rabbit.id, rabbit);
        }
        data.rabbits.get(rabbit.id)?.historicalData.set(j, { tile: tile.id });
      }
      offset = baseRabbitsOffset + rabbitsCount * rabbitSize;

      const foxesCount = nextInt64();

      let baseFoxesOffset = offset;
      for (let k = 0; k < foxesCount; k++) {
        offset = baseFoxesOffset + k * foxSize;

        const fox = {
          birthDay: nextInt64(),
          id: nextUInt64(),
          historicalData: new Map(),
        } as Fox;
        const hunger = nextDouble();

        if (!data.foxes.has(fox.id)) {
          data.foxes.set(fox.id, fox);
        }
        data.foxes
          .get(fox.id)
          ?.historicalData.set(j, { tile: tile.id, hunger });
      }
      offset = baseFoxesOffset + foxesCount * foxSize;

      tile.historicalDatas.push(tileData);
    }
    data.tiles.set(tile.id, tile);
  }

  console.log(data.foxes, data.rabbits);
  return data;
}
