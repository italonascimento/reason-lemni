type emitter('a) = {
  .
  [@bs.meth] "signal": 'a => unit
};

type jsSelf('p, 's) = {
  .
  "props": 'p,
  "state": 's,
};

type jsSources('p, 's) = {
  .
  "props": Xs.stream('p),
  "state": Xs.stream('s),
};

[@bs.deriving jsConverter]
type sources('p, 's) = {
  props: Xs.stream('p),
  state: Xs.stream('s),
};

type jsSinks('p, 's) = {
  .
  "initialState": 's,
  "stateReducer": Xs.stream('s => 's),
  "view": jsSelf('p, 's) => ReasonReact.reactElement,
};

[@bs.deriving jsConverter]
type sinks('p, 's) = {
  initialState: 's,
  stateReducer: Xs.stream('s => 's),
  view: jsSelf('p, 's) => ReasonReact.reactElement,
};

type mainFunction('p, 's) = jsSources('p, 's) => jsSinks('p, 's);

type lemniComponent('p, 's) = (mainFunction('p, 's)) => ReasonReact.reactClass;

[@bs.module "@lemni/core"] external lemni: lemniComponent('p, 's) = "lemni";

[@bs.send] external emitter: jsSelf('p, 's) => Xs.stream('a) => emitter('a) = "emitter";

let wrapper = main => sources =>
  sinksToJs @@ main @@ sourcesFromJs @@  sources;

let make = main => lemni @@ wrapper @@ main;

