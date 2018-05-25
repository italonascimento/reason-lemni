type sources('p, 's) = {
  props: Xs.stream('p),
  state: Xs.stream('s),
};

type sinks('p, 's) = {
  initialState: unit => 's,
  stateReducer: Xs.stream('s => 's),
  view: (~props: 'p, ~state: 's) => ReasonReact.reactElement,
};


type component('p, 's) = sources('p, 's) => sinks('p, 's);

type reducerFn('a) = 'a => 'a;

type action('a) =
  | Reduce(reducerFn('a))
;

let l = (~component: component('p, 's), ~props) => {
  let baseComponent = ReasonReact.reducerComponent("Lemni");
  let sources = {
    props: Xs.create(),
    state: Xs.create(),
  };

  let sinks = component @@ sources;

  {
    ...baseComponent,

    initialState: sinks.initialState,

    didMount: self => {
      let stateReducerListener: Xs.listener('a) = {
        next: reducer => self.send(Reduce(reducer))
      };

      sinks.stateReducer
        |> Xs.subscribe(stateReducerListener);

      self.onUnmount(() =>
        sinks.stateReducer
          |> Xs.removeListener(stateReducerListener)
      );
    },

    didUpdate: _ =>
      sources.props
        |> Xs.shamefullySendNext(props)
    ,

    reducer: (action, state) =>
      switch (action) {
        | Reduce(reducer) => ReasonReact.Update(reducer(state))
      },

    render: self => {
      sinks.view(~props=props, ~state=self.state);
    }
  };
};
