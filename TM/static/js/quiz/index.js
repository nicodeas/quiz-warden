import { getQuestion } from "./getQuestion.js";
import "./events.js";
import { renderState } from "./renderState.js";
import { getState } from "../utils/getState.js";

let currentQuestionIndex = 1;

// set question index based on query string "question"
const urlParams = new URLSearchParams(window.location.search);
const qs = urlParams.get("question");
if (qs) {
  currentQuestionIndex = qs;
}

export const question = await getQuestion(currentQuestionIndex);

export const state = await getState();

if (state.completed) {
  window.location.href = "/result";
}

await renderState(true, state);
