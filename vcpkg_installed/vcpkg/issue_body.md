Package: glad[core,gl-api-10,gl-api-11,gl-api-12,gl-api-13,gl-api-14,gl-api-15,gl-api-20,gl-api-21,gl-api-30,gl-api-31,gl-api-32,gl-api-33,gl-api-40,gl-api-41,gl-api-42,gl-api-43,gl-api-44,gl-api-45,gl-api-46,loader]:x64-windows@0.1.36

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.41.34120.0
-    vcpkg-tool version: 2024-07-10-d2dfc73769081bdd9b782d08d27794780b7a99b9
    vcpkg-scripts version: unknown

**To Reproduce**

`vcpkg install `

**Failure logs**

```
-- Note: glad only supports static library linkage. Building static library.
-- Downloading https://github.com/Dav1dde/glad/archive/1ecd45775d96f35170458e6b148eb0708967e402.tar.gz -> Dav1dde-glad-1ecd45775d96f35170458e6b148eb0708967e402.tar.gz...
[DEBUG] To include the environment variables in debug output, pass --debug-env
[DEBUG] Trying to load bundleconfig from D:\Repo\vcpkg\vcpkg-bundle.json
[DEBUG] Failed to open: D:\Repo\vcpkg\vcpkg-bundle.json
[DEBUG] Bundle config: readonly=false, usegitregistry=false, embeddedsha=nullopt, deployment=Git, vsversion=nullopt
[DEBUG] Metrics enabled.
[DEBUG] Feature flag 'binarycaching' unset
[DEBUG] Feature flag 'compilertracking' unset
[DEBUG] Feature flag 'registries' unset
[DEBUG] Feature flag 'versions' unset
[DEBUG] Feature flag 'dependencygraph' unset
warning: Download failed -- retrying after 1000ms
warning: Download failed -- retrying after 2000ms
warning: Download failed -- retrying after 4000ms
error: Missing Dav1dde-glad-1ecd45775d96f35170458e6b148eb0708967e402.tar.gz and downloads are blocked by x-block-origin.
error: https://github.com/Dav1dde/glad/archive/1ecd45775d96f35170458e6b148eb0708967e402.tar.gz: WinHttpSendRequest failed with exit code 12002
error: https://github.com/Dav1dde/glad/archive/1ecd45775d96f35170458e6b148eb0708967e402.tar.gz: WinHttpSendRequest failed with exit code 12002
error: https://github.com/Dav1dde/glad/archive/1ecd45775d96f35170458e6b148eb0708967e402.tar.gz: WinHttpSendRequest failed with exit code 12002
error: https://github.com/Dav1dde/glad/archive/1ecd45775d96f35170458e6b148eb0708967e402.tar.gz: WinHttpSendRequest failed with exit code 12002
[DEBUG] D:\a\_work\1\s\src\vcpkg\base\downloads.cpp(1030): 
[DEBUG] Time in subprocesses: 0us
[DEBUG] Time in parsing JSON: 6us
[DEBUG] Time in JSON reader: 0us
[DEBUG] Time in filesystem: 1887us
[DEBUG] Time in loading ports: 0us
[DEBUG] Exiting after 1.5 min (91360024us)

CMake Error at scripts/cmake/vcpkg_download_distfile.cmake:32 (message):
      
      Failed to download file with error: 1
      If you are using a proxy, please check your proxy setting. Possible causes are:
      
      1. You are actually using an HTTP proxy, but setting HTTPS_PROXY variable
         to `https://address:port`. This is not correct, because `https://` prefix
         claims the proxy is an HTTPS proxy, while your proxy (v2ray, shadowsocksr
         , etc..) is an HTTP proxy. Try setting `http://address:port` to both
         HTTP_PROXY and HTTPS_PROXY instead.
      
      2. If you are using Windows, vcpkg will automatically use your Windows IE Proxy Settings
         set by your proxy software. See https://github.com/microsoft/vcpkg-tool/pull/77
         The value set by your proxy might be wrong, or have same `https://` prefix issue.
      
      3. Your proxy's remote server is out of service.
      
      If you've tried directly download the link, and believe this is not a temporary
      download server failure, please submit an issue at https://github.com/Microsoft/vcpkg/issues
      to report this upstream download server failure.
      

Call Stack (most recent call first):
  scripts/cmake/vcpkg_download_distfile.cmake:270 (z_vcpkg_download_distfile_show_proxy_and_fail)
  scripts/cmake/vcpkg_from_github.cmake:106 (vcpkg_download_distfile)
  ports/glad/portfile.cmake:3 (vcpkg_from_github)
  scripts/ports.cmake:192 (include)



```

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "dependencies": [
    {
      "name": "glad",
      "features": [
        "gl-api-46"
      ]
    },
    "glm",
    "opengl",
    "soil2",
    "cute-headers"
  ]
}

```
</details>
