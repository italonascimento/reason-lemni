/* TODO: implement other fields of listener */
type jsListener('a) = {
  ."next": 'a => unit
};

type stream('a);

type xs('a) = {
  .
  [@bs.meth] "create": unit => stream('a)
};


type ofType('a) = 'a => stream('a);
type mapType('a, 'b) = 'a => 'b => stream('b);

[@bs.module "xstream"] external xs: xs('a) = "default";

let create = () => xs##create();

[@bs.get_index] external getOf: xs('a) => ([@bs.as "of"] _) => ofType('a) = "";
let streamOf = a => getOf(xs)(a);

[@bs.send.pipe: stream('a)] external map: ('a => 'b) => stream('b) = "map";

/* TODO: return subscription instead o unit */
[@bs.send.pipe: stream('a)] external jsSubscribe: jsListener('a) => unit = "subscribe";

let subscribe = (~next: 'a => unit) =>
  (s: stream('a)) =>
    s |>
      jsSubscribe({
        "next": v => next(v)
      });


