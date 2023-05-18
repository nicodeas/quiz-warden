import { getQuestion } from "./getQuestion.js";
import "./events.js";
import "./renderResult.js";
import { renderState } from "./renderState.js";
import { getState } from "../utils/getState.js";

let currentQuestionIndex = 1;

// set question index based on query string "question"
const urlParams = new URLSearchParams(window.location.search);
const qs = urlParams.get("question");
if (qs) {
  currentQuestionIndex = qs;
}

getQuestion(currentQuestionIndex);

export const state = await getState();

renderState(true, state);
