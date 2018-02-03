////
//// Created by Rogiel Sulzbach on 10/14/17.
////
//
//public protocol Resource2 {
//
//    /// The resource name
//    var name: String { get }
//
//    /// The resource type
//    var type: ResourceType { get }
//
//}
//
//public protocol NumericResource : Resource {
//    /// The resource value
//    var value: Double { get set }
//
//    /// Creates a new empty resource
//    init()
//
//    /// Creates a new resource
//    init(value: Double)
//}
//
//public enum ResourceType {
//
//    case integer
//    case numeric
//    case boolean
//    case custom (type: CustomResourceValue.Type)
//
//}
//
//public protocol CustomResourceValue {
//
//}