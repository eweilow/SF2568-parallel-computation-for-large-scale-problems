import {
  useCallback,
  ChangeEvent,
  useState,
  useEffect,
  useMemo,
  useContext,
  createContext,
} from "react";
import { processData, Tile } from "../lib/data";

function BufferInput(props: { onBufferChange: (buffer: ArrayBuffer) => void }) {
  const onChange = useCallback(
    (e: ChangeEvent<HTMLInputElement>) => {
      var files = e.target.files;

      var reader = new FileReader();
      reader.onload = (ev) => {
        props.onBufferChange(reader.result as ArrayBuffer);
      };
      reader.readAsArrayBuffer(files[0]);
    },
    [props.onBufferChange]
  );
  return <input type="file" onChange={onChange} />;
}

function Tree(props: { x: number; y: number; rotate: number; size: number }) {
  return (
    <g
      transform={`translate(${props.x + props.size / 2} ${
        props.y + props.size / 2
      }) rotate(${props.rotate})`}
    >
      <image
        x={-props.size / 2}
        y={-props.size / 2}
        width={props.size}
        height={props.size}
        href="https://opengameart.org/sites/default/files/styles/medium/public/_tree_02_prev_0.png"
      />
    </g>
  );
}

function useRNG(seed: number) {
  return () => {
    // https://stackoverflow.com/a/19303725
    var x = Math.sin(seed++) * 10000;
    return x - Math.floor(x);
  };
}

function Trees(props: {
  tileX: number;
  tileY: number;
  tileSize: number;
  probability: number;
}) {
  const rng = useRNG(props.tileX * 1e5 + props.tileY);

  const n = 8;

  const trees = [];
  const hasTrees = new Set<number>();

  const dp = 0.1;
  for (let p = 0; p < props.probability; p += dp) {
    for (let i = 0; i < n; i++) {
      for (let j = 0; j < n; j++) {
        const key = i * n + j;

        if (rng() <= dp) {
          if (hasTrees.has(key)) {
            continue;
          }
          const baseSize = props.tileSize / n;
          const size = props.tileSize * ((rng() * 1) / n + 1 / n);

          const px = (rng() * 2 - 1) * 0.25;
          const py = (rng() * 2 - 1) * 0.25;
          const x = (px / n + i / n) * props.tileSize;
          const y = (-0.05 + py / n + j / n) * props.tileSize;
          trees.push(
            <Tree
              x={props.tileX + x + (baseSize - size) / 2}
              y={props.tileY + y + (baseSize - size) / 2}
              rotate={rng() * 4 - 2}
              size={size}
              key={key}
            />
          );
          hasTrees.add(key);
        }
      }
    }
  }
  return <>{trees}</>;
}

function TreeDisplay({ tile, tileSize }: { tileSize: number; tile: Tile }) {
  const x = tile.x - tileSize / 2;
  const y = tile.y - tileSize / 2;

  const ts = useContext(TimestepContext);
  const growth = useContext(GrowthFactorContext);
  return (
    <g>
      {!tile.isWaterTile && (
        <Trees
          tileX={x}
          tileY={y}
          tileSize={tileSize}
          probability={tile.historicalDatas[ts].vegetation * growth}
        />
      )}
    </g>
  );
}

function TileDisplay({ tile, tileSize }: { tileSize: number; tile: Tile }) {
  const x = tile.x - tileSize / 2;
  const y = tile.y - tileSize / 2;

  const ts = useContext(TimestepContext);
  const growth = useContext(GrowthFactorContext);
  return (
    <g>
      <image
        x={x}
        y={y}
        width={tileSize}
        height={tileSize}
        href={
          tile.isWaterTile
            ? "https://encrypted-tbn0.gstatic.com/images?q=tbn%3AANd9GcSEampVTPTNriTpc2GYhFdkLZt9qU4wodwrt5UWMPa3DWD8v4V1&usqp=CAU"
            : "https://i.pinimg.com/originals/07/c2/b8/07c2b8bcad9fb6a6efb8d3c62e5f3080.jpg"
        }
      />
    </g>
  );
}

function Display(props: { buffer: ArrayBuffer }) {
  const data = useMemo(() => {
    return processData(props.buffer);
  }, [props.buffer]);

  const maxX = useMemo(() => {
    return Math.max(...data.tiles.map((el) => el.x));
  }, [data]);
  const maxY = useMemo(() => {
    return Math.max(0, ...data.tiles.map((el) => el.x));
  }, [data]);
  const minDataCount = useMemo(() => {
    return Math.max(0, ...data.tiles.map((el) => el.historicalDataCount));
  }, [data]);

  const [ts, setTs] = useState<number>(0);
  const [growth, setGrowth] = useState<number>(1);

  return (
    <div>
      Buffer length: {props.buffer.byteLength}. Tiles: {data.tiles.length}
      {maxX}x{maxY}
      <input
        type="range"
        value={ts}
        onChange={(e) => setTs(e.target.valueAsNumber)}
        min={0}
        max={Math.max(0, minDataCount - 1)}
      />
      <input
        type="range"
        value={growth}
        onChange={(e) => setGrowth(e.target.valueAsNumber)}
        min={0}
        max={1}
        step={1 / 10}
      />
      <hr />
      <TimestepContext.Provider value={ts}>
        <GrowthFactorContext.Provider value={growth}>
          <svg
            viewBox={`${data.tileSize / 2} ${
              data.tileSize / 2
            } ${maxX} ${maxY}`}
            width={800}
          >
            {data.tiles.map((tile) => (
              <TileDisplay key={tile.id} tile={tile} tileSize={data.tileSize} />
            ))}
            {data.tiles.map((tile) => (
              <TreeDisplay key={tile.id} tile={tile} tileSize={data.tileSize} />
            ))}
          </svg>
        </GrowthFactorContext.Provider>
      </TimestepContext.Provider>
      <style jsx>{`
        svg {
          display: block;
          margin: 0 auto;
          border: 1px solid black;
        }
      `}</style>
    </div>
  );
}

const TimestepContext = createContext<number>(0);
const GrowthFactorContext = createContext<number>(1);

function Page() {
  const [buffer, setBuffer] = useState<ArrayBuffer>(null);

  const onBufferChange = useCallback((buf: ArrayBuffer) => {
    setBuffer(buf);
  }, []);

  return (
    <div>
      <BufferInput onBufferChange={onBufferChange} />

      {buffer == null && <h1>Upload a file!</h1>}
      {buffer != null && (
        <>
          <Display buffer={buffer} />
        </>
      )}
    </div>
  );
}

export default Page;
