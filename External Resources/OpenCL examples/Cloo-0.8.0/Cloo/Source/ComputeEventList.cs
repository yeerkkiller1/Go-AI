﻿#region License

/*

Copyright (c) 2009 - 2010 Fatjon Sakiqi

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

*/

#endregion

namespace Cloo
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using Cloo.Bindings;

    /// <summary>
    /// Represents a list of OpenCL generated or user created events.
    /// </summary>
    /// <seealso cref="ComputeCommandQueue"/>
    public class ComputeEventList : IList<ComputeEventBase>
    {
        #region Fields

        private readonly List<ComputeEventBase> events;

        #endregion

        #region Constructors

        /// <summary>
        /// Creates an empty <c>ComputeEventList</c>.
        /// </summary>
        public ComputeEventList()
        {
            events = new List<ComputeEventBase>();
        }

        /// <summary>
        /// Creates a new <c>ComputeEventList</c> from an existing list of <c>ComputeEventBase</c>s.
        /// </summary>
        /// <param name="events"> A list of <c>ComputeEventBase</c>s. </param>
        public ComputeEventList(IList<ComputeEventBase> events)
        {
            events = new Collection<ComputeEventBase>(events);
        }

        #endregion

        #region Properties

        /// <summary>
        /// Gets the last event on the list.
        /// </summary>
        public ComputeEventBase Last { get { return events[events.Count - 1]; } }

        #endregion

        #region Public methods

        /// <summary>
        /// Waits on the host thread for the specified events to complete.
        /// </summary>
        /// <param name="events"> The events to be waited for completition. </param>
        public static void Wait(ICollection<ComputeEventBase> events)
        {
            unsafe
            {
                ComputeErrorCode error = CL10.WaitForEvents(events.Count, Tools.ExtractHandles(events));
                ComputeException.ThrowOnError(error);
            }
        }

        /// <summary>
        /// Waits on the host thread for the <c>ComputeEventBase</c>s in the <c>ComputeEventList</c> to complete.
        /// </summary>
        public void Wait()
        {
            unsafe
            {
                ComputeErrorCode error = CL10.WaitForEvents(events.Count, Tools.ExtractHandles(events));
                ComputeException.ThrowOnError(error);
            }
        }

        #endregion

        #region IList<ComputeEventBase> Members

        /// <summary>
        /// 
        /// </summary>
        /// <param name="item"></param>
        /// <returns></returns>
        public int IndexOf(ComputeEventBase item)
        {
            return events.IndexOf(item);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="index"></param>
        /// <param name="item"></param>
        public void Insert(int index, ComputeEventBase item)
        {
            events.Insert(index, item);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="index"></param>
        public void RemoveAt(int index)
        {
            events.RemoveAt(index);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public ComputeEventBase this[int index]
        {
            get
            {
                return events[index];
            }
            set
            {
                events[index] = value;
            }
        }

        #endregion

        #region ICollection<ComputeEventBase> Members

        /// <summary>
        /// 
        /// </summary>
        /// <param name="item"></param>
        public void Add(ComputeEventBase item)
        {
            events.Add(item);
        }

        /// <summary>
        /// 
        /// </summary>
        public void Clear()
        {
            events.Clear();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="item"></param>
        /// <returns></returns>
        public bool Contains(ComputeEventBase item)
        {
            return events.Contains(item);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="array"></param>
        /// <param name="arrayIndex"></param>
        public void CopyTo(ComputeEventBase[] array, int arrayIndex)
        {
            events.CopyTo(array, arrayIndex);
        }

        /// <summary>
        /// 
        /// </summary>
        public int Count
        {
            get { return events.Count; }
        }

        /// <summary>
        /// 
        /// </summary>
        public bool IsReadOnly
        {
            get { return false; }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="item"></param>
        /// <returns></returns>
        public bool Remove(ComputeEventBase item)
        {
            return events.Remove(item);
        }

        #endregion

        #region IEnumerable<ComputeEventBase> Members

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public IEnumerator<ComputeEventBase> GetEnumerator()
        {
            return ((IEnumerable<ComputeEventBase>)events).GetEnumerator();
        }

        #endregion

        #region IEnumerable Members

        IEnumerator IEnumerable.GetEnumerator()
        {
            return ((IEnumerable)events).GetEnumerator();
        }

        #endregion
    }
}