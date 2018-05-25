// Generated by BUCKLESCRIPT VERSION 3.0.0, PLEASE EDIT WITH CARE

import * as Curry from "bs-platform/lib/es6/curry.js";
import * as Core from "@lemni/core";

function sourcesToJs(param) {
  return {
          props: param[/* props */0],
          state: param[/* state */1]
        };
}

function sourcesFromJs(param) {
  return /* record */[
          /* props */param.props,
          /* state */param.state
        ];
}

function sinksToJs(param) {
  return {
          initialState: param[/* initialState */0],
          stateReducer: param[/* stateReducer */1],
          view: param[/* view */2]
        };
}

function sinksFromJs(param) {
  return /* record */[
          /* initialState */param.initialState,
          /* stateReducer */param.stateReducer,
          /* view */param.view
        ];
}

function wrapper(main, sources) {
  return sinksToJs(Curry._1(main, sourcesFromJs(sources)));
}

function make(main) {
  return Curry._1(Core.lemni, (function (param) {
                return sinksToJs(Curry._1(main, sourcesFromJs(param)));
              }));
}

export {
  sourcesToJs ,
  sourcesFromJs ,
  sinksToJs ,
  sinksFromJs ,
  wrapper ,
  make ,
  
}
/* @lemni/core Not a pure module */