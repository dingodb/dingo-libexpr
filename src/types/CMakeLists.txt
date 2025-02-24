# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(SRCS
    decimal/decimal.cc
        decimal/decimal_p.cc
)

include_directories(${GMP_BINARY_PATH}/install/include)
include_directories(${CMAKE_SOURCE_DIR}/src/expr)

add_library(${TYPES_LIB_NAME} STATIC ${SRCS})
target_link_libraries(${TYPES_LIB_NAME} ${GMP_LIB_NAME} ${GMPXX_LIB_NAME})
add_dependencies(${TYPES_LIB_NAME} gmp)
