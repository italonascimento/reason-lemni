type sources('p, 's) = {
  props: Xs.stream('p),
  state: Xs.stream('s),
};

type viewArgs('p, 's) = {
  props: 'p,
  state: 's
};

type sinks('p, 's) = {
  initialState: unit => 's,
  stateReducer: Xs.stream(('s) => 's),
  view: viewArgs('p, 's) => ReasonReact.reactElement,
};


type component('p, 's) = sources('p, 's) => sinks('p, 's);

type reducerFn('a) = 'a => 'a;

type action('a) =
  | Reduce(reducerFn('a))
;

let l = (~component: component('p, 's), ~props) => {
  let baseComponent = ReasonReact.reducerComponent("Lemni");
  let sources: sources('p, 's) = {
    props: Xs.createWithMemory(),
    state: Xs.createWithMemory(),
  };

  let sinks = component @@ sources;

  {
    ...baseComponent,

    initialState: sinks.initialState,

    didMount: self => {
      let stateReducerNext = reducer => self.send(Reduce(reducer));

      sinks.stateReducer
        |> Xs.subscribe(~next=stateReducerNext, ());

      self.onUnmount(() =>
        sinks.stateReducer
          |> Xs.removeListener(~next=stateReducerNext, ())
      );
    },

    reducer: (action, state) =>
      switch (action) {
        | Reduce(reducer) => ReasonReact.Update(reducer(state))
      },

    render: self => {
      sources.props
        |> Xs.shamefullySendNext(props);
      sinks.view({props, state: self.state});
    }
  };
};
