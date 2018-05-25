[@bs.deriving jsConverter]
type props = {
  name: string
};

[@bs.deriving jsConverter]
type state = {
  count: int,
  label: string,
};

let adaptReducer =
  reducer =>
    jsState =>
      jsState
        |> stateFromJs
        |> reducer
        |> stateToJs
;

let adaptView =
  view =>
    self =>{
      let props = propsFromJs(self##props);
      let state = stateFromJs(self##state);
      let emitter =  Lemni.emitter(self);

      view(~props, ~state, ~emitter);
    };

let countReducer =
  state => {
    ...state,
    count: state.count + 1,
  };


let reactClass = Lemni.make(_sources => {
  let increment = Xs.create();

  {
    initialState: {
      count: 0,
      label: "Test",
    } |> stateToJs,

    stateReducer: increment |>
      Xs.map(_ => adaptReducer @@ countReducer)
    ,

    view: adaptView @@ (~props, ~state, ~emitter) =>
      <div>
        {ReasonReact.string(props.name)}
        {ReasonReact.string(": ")}
        {ReasonReact.string(string_of_int(state.count))}

        <div>
          <button onClick={(e) => (emitter(increment))##signal(e)}>
            {ReasonReact.string(state.label)}
          </button>
        </div>
      </div>
  };
});


let make = (~name, children) =>
  ReasonReact.wrapJsForReason(
    ~props={"name": name},
    ~reactClass,
    children
  );
  