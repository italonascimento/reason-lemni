/* TODO: implement other fields of listener */
type jsListener('a) = {
  ."next": 'a => unit
};

[@bs.deriving jsConverter]
type listener('a) = {
  next: 'a => unit
};

type stream('a);

type xs('a) = {
  .
  [@bs.meth] "create": unit => stream('a),
  [@bs.meth] "createWithMemory": unit => stream('a),
};


type ofType('a) = 'a => stream('a);
type mapType('a, 'b) = 'a => 'b => stream('b);

[@bs.module "xstream"] external xs: xs('a) = "default";

let create = () => xs##create();

let createWithMemory = () => xs##createWithMemory();

[@bs.get_index] external getOf: xs('a) => ([@bs.as "of"] _) => ofType('a) = "";
let streamOf = a => getOf(xs)(a);

type operator('a, 'b) = stream('a) => 'b;

[@bs.module "xstream/extra/sampleCombine"] external sampleCombine: (. stream('b)) => ((. stream('a)) => stream(('a, 'b))) = "default";

[@bs.send.pipe: stream('a)] external map: ('a => 'b) => stream('b) = "map";

[@bs.send.pipe: stream('a)] external debug: ('a => unit) => stream('a) = "debug";

[@bs.send.pipe: stream('a)] external compose: operator('a, 'b) => 'b = "compose";

[@bs.send.pipe: stream('a)] external shamefullySendNext: ('a) => unit = "shamefullySendNext";

/* TODO: return subscription instead o unit */
[@bs.send.pipe: stream('a)] external jsSubscribe: jsListener('a) => unit = "subscribe";
let subscribe = (listener: listener('a)) =>
  (s: stream('a)) =>
    s |>
      jsSubscribe @@ listenerToJs @@ listener;

[@bs.send.pipe: stream('a)] external jsRemoveListener: jsListener('a) => unit = "removeListener";
let removeListener = (listener: listener('a)) =>
  (s: stream('a)) =>
    s |>
      jsRemoveListener @@ listenerToJs @@ listener;
