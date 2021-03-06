type props = {
  step: int
};

type state = {
  count: int,
  label: string,
};
  

let countReducer =
  step =>
    state => {
      ...state,
      count: state.count + step,
    };

let component: Lemni.component(props, state) = sources => {
  let increment = Xs.create();

  {
    initialState: () => {
      count: 0,
      label: "Test",
    },

    stateReducer: increment
    |> Xs.sampleCombine(
        sources.props
        |> Xs.map(p => p.step)
      )
    |> Xs.map(((_, step)) => step)
    |> Xs.map(step => countReducer(step)),

    view: ({state: state}) => {
      <div>
        {ReasonReact.string("Count: ")}
        {ReasonReact.string(string_of_int(state.count))}

        <div>
          <button onClick={e => increment |> Xs.shamefullySendNext(e)}>
            {ReasonReact.string(state.label)}
          </button>
        </div>
      </div>
    }
  };
};

let make = (~step, _children) => {
  Lemni.l(
    ~component,
    ~props={step: step},
  )
}
  