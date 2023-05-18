const loginUser = async () => {
  
  const username = document.getElementById("username");
  const password = document.getElementById("password");

  const credentials = {
    username: username.value,
    password: password.value,
  };
  // reset input values
  username.value = "";
  password.value = "";

  let res;
  try {
    res = await fetch("http://localhost:8000/api/login", {
      headers: {
        Accept: "application/json",
        "Content-Type": "application/json",
      },
      method: "POST",
      body: JSON.stringify(credentials),
    });
  } catch (err) {
    console.log(err);
  }

  if (res.status === 200) {
    window.location.href = "http://localhost:8000";
  } else {
    const loginError = document.getElementById("login-error");
    loginError.innerHTML =
      "Failed to login: Invalid credentials";
  }
};

const logoutUser = async (e) => {
  try {
    await fetch("http://localhost:8000/api/logout", {});
    window.location.href = "http://localhost:8000/login";
  } catch (err) {
    console.log(err);
  }
};
