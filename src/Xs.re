type stream('a);
type error;

type jsSubscription = {
  .
  [@bs.meth] "unsubscribe": unit => unit
};

type subscription = {
  unsubscribe: unit => unit
};

type jsListener('a) = {
  .
  "next": Js.nullable('a => unit),
  "error": Js.nullable(error => unit),
  "complete": Js.nullable(unit => unit),
};

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

[@bs.module "xstream/extra/sampleCombine"] external jsSampleCombine: (. stream('b)) => ((. stream('a)) => stream(('a, 'b))) = "default";
let sampleCombine =
  combined =>
    source =>
      jsSampleCombine(.combined)(.source);

[@bs.send.pipe: stream('a)] external map: ('a => 'b) => stream('b) = "map";

[@bs.send.pipe: stream('a)] external filter: ('a => bool) => stream('a) = "filter";

[@bs.send.pipe: stream('a)] external debug: ('a => unit) => stream('a) = "debug";

[@bs.send.pipe: stream('a)] external shamefullySendNext: ('a) => unit = "shamefullySendNext";

/* TODO: return subscription instead o unit */
[@bs.send.pipe: stream('a)] external jsSubscribe: jsListener('a) => jsSubscription = "subscribe";
let subscribe = (
  ~next: option('a => unit)=?,
  ~error: option(error => unit)=?,
  ~complete: option(unit => unit)=?,
  ()
) =>
  (s: stream('a)) => {
    let jsSubscription = 
      s |>
        jsSubscribe @@ {
          "next": switch (next) {
          | None => Js.Nullable.null
          | Some(next) => Js.Nullable.return(next)
          },
          "error": switch (error) {
          | None => Js.Nullable.null
          | Some(error) => Js.Nullable.return(error)
          },
          "complete": switch (complete) {
          | None => Js.Nullable.null
          | Some(complete) => Js.Nullable.return(complete)
          },
        };

    {
      unsubscribe: () => jsSubscription##unsubscribe()
    };
  };

[@bs.send.pipe: stream('a)] external jsRemoveListener: jsListener('a) => unit = "removeListener";
let removeListener = (
  ~next: option('a => unit)=?,
  ~error: option(error => unit)=?,
  ~complete: option(unit => unit)=?,
  ()
) =>
  (s: stream('a)) =>
    s |>
      jsRemoveListener @@ {
        "next": switch next {
        | None => Js.Nullable.null
        | Some(next) => Js.Nullable.return(next)
        },
        "error": switch error {
        | None => Js.Nullable.null
        | Some(error) => Js.Nullable.return(error)
        },
        "complete": switch complete {
        | None => Js.Nullable.null
        | Some(complete) => Js.Nullable.return(complete)
        },
      };

