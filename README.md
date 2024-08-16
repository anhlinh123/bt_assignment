# Booking backend

## Design considerations

This is an HTTP backend. Multiple alternatives were considered:

- Embedded backend (in a client process)

This way we can simplify the client-server communication via direct function calls.

The drawbacks are mostly about flexibility. Each client is attached to a separate backend, which limits what we can do:

    - Concurrent access to the same backend is weird to be implemented.
    - We have to implement a client to test the backend.

- Using IPC mechanisms (sockets, gRPC, ...)

This approach is more flexible, as the client and server can be developed independently.

The catch is that we have to implement the protocols, which can be time-consuming. We also have to implement at least one client to verify the APIs.

Using HTTP has several advantages:

- Drogon framework is handy so we don't need to care about client-server communication
- The protocol is simple as it is a RESTful service.
- No need to develop a client. We can use a browser, `curl`, or any HTTP tools.
- Adding functionalities to the backend is straightforward.

At the same time, some disadvantages should be taken into account:

- Drogon isn't well-documented so it takes (and has taken) much time to get familiar with.
- HTTP communication is slower than IPC and direct function calls.

## Development

### Setup

Make sure `docker` is installed in your dev machine.

To set up the dev environment, run: `scripts/dev_env.sh`.

The script will download all necessary tools and launch an interactive container, in which everything is ready for development.

### Build

The project uses `conan` to manage dependencies, which should be installed first.
```
conan install . --output-folder=<build_dir> --build=missing
```

Using cmake preset is preferable:
```
cmake -S <project_dir> -B <build_dir> --preset=conan-release
cmake --build <build_dir> --preset=conan-release
```

### Run

The output binary is `server`, which is an HTTP server. It can be launched by running:
```
<path>/server <path to config.json>
```

`config.json` is a drogon-specific file to configure the server. More details can be found [here](https://github.com/drogonframework/drogon/blob/master/config.example.json).

There is a special info that is specific to the backend, which is located under the `custom_config` section.

It must specify the path to the JSON file containing the database. The path is relative to the directory containing the config.json file.

```
"custom_config" : {
    "data" : <JSON file>
}
```
The default `config.json` in the source root directory sets up a server at `localhost:8080`, and uses the data file at `data/data.json`.

### REST APIs

- Get all movies: `GET /movies`
- Get all theaters showing a movie: `GET /theaters?movie={}`
- Get all available seats for a movie in a theater: `GET /seats?theaters={}&movie={}`
- Book some seats: `POST /book?theater={}&movie={}&seats{colon-separated list}`

### Test

```
ctest --test-dir <build_dir> --preset=conan-release
```

### Code format

Code formatting, linting, schema checking... usually need a more holistic solution.

For this scope, there is a simplistic script to format C++ and JSON files.
```
scripts/code_format.sh
```
